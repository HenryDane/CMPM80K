#include "game.h"
#include "util.h"
#include <iostream>

Dialogue* ark_first;
Dialogue* ark_ready;
bool has_ark_first;
bool has_ark_second;

void init_game() {
    ark_first = new Dialogue(-1, -1, "Ark");
    ark_first->add_text("This is the ark. When you collide with it while you have more than zero planks, it gets stronger.");
    ark_first->add_text("The number of planks it has is shown as its HP in the top right.");
    ark_first->add_text("The ark needs to have at least 15HP before you can give it animals.");
    ark_ready = new Dialogue(-1, -1, "Ark");
    ark_ready->add_text("The ark is now sturdy enough to host animals.");
    ark_ready->add_text("Remember, you need two of each kind.");
    has_ark_first = false;
    has_ark_second = false;
}

void exit_game() {
    delete ark_first;
    delete ark_ready;
}

void game_check_win_loose() {
    if (player->get_hearts() <= 0 ||
        game->get_turns_remaining() <= 0 ||
        ark.planks_count < 0) {
        // LOSS
        game->alter_game_state(GameManager::LOOSE_CUTSCENE);
    }

    if (ark.planks_count >= 15 && ark.cows > 1 && ark.chickens > 1 &&
        ark.pigs > 1 && ark.sheep > 1) {
        // WIN
        game->alter_game_state(GameManager::WIN_CUTSCENE);
    }
}

void handle_collide_wall(int dx, int dy) {
    int nx = player->get_x() + dx;
    int ny = player->get_y() + dy;

    int tile = current_map->get_tile_at(nx, ny);
    if ((tile == T_EMPTY) ||
        ((tile % 11 < 5) && (tile < 27))) {
        if (player->get_held_item_texture() == T_PLANK) {
            current_map->set_tile_at(nx, ny, T_PLANKS);
            player->set_held_item_texture(T_EMPTY);
        }
    }
}

void do_drop_item() {
    if (!is_entity_pickupable(get_entity_type_from_texture(player->get_held_item_texture()))) {
        std::cout << "reset player held item" << std::endl;
        player->set_held_item_texture(T_EMPTY);
        return;
    }

    int dx = 0;
    int dy = 0;

    if (!current_map->is_collideable(player->get_x() + 1, player->get_y(), true)) {
        dx = 1;
    } else if (!current_map->is_collideable(player->get_x() - 1, player->get_y(), true)) {
        dx = -1;
    } else if (!current_map->is_collideable(player->get_x(), player->get_y() + 1, true)) {
        dy = 1;
    } else if (!current_map->is_collideable(player->get_x(), player->get_y() - 1, true)) {
        dy = -1;
    } else {
        std::cout << "unable to place entity" << std::endl;
        return;
    }

    Entity* e = new Entity(g_next_uuid++, player->get_x() + dx, player->get_y() + dy,
                           get_entity_type_from_texture(player->get_held_item_texture()),
                           player->get_held_item_texture());
    current_map->register_entity(e);

    player->set_held_item_texture(T_EMPTY);
}

void do_pickup_item() {
    int px = player->get_x();
    int py = player->get_y();

    Entity* entity_w = current_map->check_entity_collision(px - 1, py);
    Entity* entity_e = current_map->check_entity_collision(px + 1, py);
    Entity* entity_n = current_map->check_entity_collision(px, py - 1);
    Entity* entity_s = current_map->check_entity_collision(px, py + 1);

    if (entity_n != nullptr) {
        if (is_entity_pickupable(entity_n->get_type())) {
            player->set_held_item_texture(entity_n->get_texture());
            entity_n->set_type(-1);
            return;
        }
    }
    if (entity_s != nullptr) {
        if (is_entity_pickupable(entity_s->get_type())) {
            player->set_held_item_texture(entity_s->get_texture());
            entity_s->set_type(-1);
            return;
        }
    }
    if (entity_e != nullptr) {
        if (is_entity_pickupable(entity_e->get_type())) {
            player->set_held_item_texture(entity_e->get_texture());
            entity_e->set_type(-1);
            return;
        }
    }
    if (entity_w != nullptr) {
        if (is_entity_pickupable(entity_w->get_type())) {
            player->set_held_item_texture(entity_w->get_texture());
            entity_w->set_type(-1);
            return;
        }
    }

    std::cout << "no nearby entities" << std::endl;
    return;
}

bool handle_entity_collision(Entity* e) {
    if (e->get_type() == 2) {
        player->set_coin_count(player->get_coin_count() + 1);
        e->set_type(-1);
        return false;
    } else if (e->get_type() == 20) {
        player->set_planks_count(player->get_planks_count() + 1);
        e->set_type(-1);
        return false;
    } else if (e->get_type() == 1) {
        int n_coins = (rand() % 4) + 1;
        int n_hearts = rand() % 2;
        e->set_type(-1);

        for (int i = 0; i < n_coins; i++) {
            Entity* c = new Entity(g_next_uuid++, e->get_x(), e->get_y(), 2, T_COIN);
            current_map->register_entity(c, false);
        }
        if (n_hearts > 0) {
            Entity* h = new Entity(g_next_uuid++, e->get_x(), e->get_y(), 3, T_HEART);
            current_map->register_entity(h, false);
        }
    } else if (e->get_type() == 3) {
        player->set_hearts(player->get_hearts() + 1);
        e->set_type(-1);
        return false;
    } else if (e->get_type() == E_ENEMY0 ||
               e->get_type() == E_ENEMY1 ||
               e->get_type() == E_ENEMY2) {
        std::cout << "off item texture: " << player->get_off_item_texture() << std::endl;
        if (player->get_off_item_texture() == T_AXE) {
            e->set_state(e->get_state() - 1);
        } else if (player->get_off_item_texture() == T_SWORD0) {
            e->set_state(e->get_state() - 2);
        } else if (player->get_off_item_texture() == T_SWORD1) {
            e->set_state(e->get_state() - 3);
        } else if (player->get_off_item_texture() == T_SWORD2) {
            e->set_state(e->get_state() - 4);
        } else if (player->get_off_item_texture() == T_EMPTY) {
            if (rand() % 100 < 50) {
                e->set_state(e->get_state() - 1);
            }
        }
        std::cout << "hit enemy: " << e->get_state() << std::endl;
        if (e->get_state() <= 0) {
            e->set_type(-1);
            return false;
        } else {
            return true;
        }
    } else if (e->get_type() == 21) {
        // collided with the ark
        if (player->get_planks_count() > 0) {
            ark.planks_count += player->get_planks_count();
            player->set_planks_count(0);
        }

        if (!has_ark_first) {
            has_ark_first = true;
            active_dialogue = ark_first;
            dialogue_state = 0;
            game->alter_game_state(GameManager::DIALOGUE);
            return true;
        }

        if (!has_ark_second && ark.planks_count >= 15) {
            has_ark_second = true;
            active_dialogue = ark_ready;
            dialogue_state = 0;
            game->alter_game_state(GameManager::DIALOGUE);
            return true;
        }

        if (ark.planks_count < 15) {
            // must have minimum plank count
            return true;
        }

        if ((player->get_held_item_texture() >= 41 &&
            player->get_held_item_texture() <= 43) ||
            player->get_held_item_texture() == T_CHICKEN) {
            // player is holding animal
            int type = player->get_held_item_texture() - 29;
            if (type == 12) {
                // its a cow
                ark.cows++;
            } else if (type == 13) {
                // its a pig
                ark.pigs++;
            } else if (type == 14) {
                // its a sheep
                ark.sheep++;
            } else if (type == E_CHICKEN) {
                // its a sheep
                ark.chickens++;
            }

            player->set_held_item_texture(T_EMPTY);
        }

        return true;
    } else if (e->get_type() == 30) {
        // hit a tree

        if (player->get_off_item_texture() == T_AXE && e->get_state() <= 0) {
            e->set_state(5);
            e->set_texture(T_STUMP);
            Entity* p = new Entity(g_next_uuid++, player->get_x(), player->get_y(), E_PLANKS, T_PLANK);
            current_map->register_entity(p, false);
        }

        return true;
    } else if (e->get_type() == E_AXE) {
        // hit axe item
        if (player->get_off_item_texture() == T_EMPTY) {
            player->set_off_item_texture(T_AXE);
        } else {
            int tx = player->get_off_item_texture();
            int t = get_entity_type_from_texture(tx);
            player->set_off_item_texture(T_AXE);
            Entity* f = new Entity(g_next_uuid++, player->get_x(), player->get_y(), t, tx);
            current_map->register_entity(f, false);
        }
        e->set_type(-1);
    } else if (e->get_type() == E_SWORD0) {
        // hit sword 0 item
        if (player->get_off_item_texture() == T_EMPTY) {
            player->set_off_item_texture(T_SWORD0);
        } else {
            int tx = player->get_off_item_texture();
            int t = get_entity_type_from_texture(tx);
            player->set_off_item_texture(T_SWORD0);
            Entity* f = new Entity(g_next_uuid++, player->get_x(), player->get_y(), t, tx);
            current_map->register_entity(f, false);
        }
        e->set_type(-1);
    } else if (e->get_type() == E_SWORD1) {
        // hit sword 1 item
        if (player->get_off_item_texture() == T_EMPTY) {
            player->set_off_item_texture(T_SWORD1);
        } else {
            int tx = player->get_off_item_texture();
            int t = get_entity_type_from_texture(tx);
            player->set_off_item_texture(T_SWORD1);
            Entity* f = new Entity(g_next_uuid++, player->get_x(), player->get_y(), t, tx);
            current_map->register_entity(f, false);
        }
        e->set_type(-1);
    } else if (e->get_type() == E_SWORD2) {
        // hit sword 2 item
        if (player->get_off_item_texture() == T_EMPTY) {
            player->set_off_item_texture(T_SWORD2);
        } else {
            int tx = player->get_off_item_texture();
            int t = get_entity_type_from_texture(tx);
            player->set_off_item_texture(T_SWORD2);
            Entity* f = new Entity(g_next_uuid++, player->get_x(), player->get_y(), t, tx);
            current_map->register_entity(f, false);
        }
        e->set_type(-1);
    }

    return false;
}

bool update_player(int dx, int dy) {
    int nx = player->get_x() + dx;
    int ny = player->get_y() + dy;

    // TODO fix this (sign compare warning)
    if (nx < 0 || nx > current_map->get_width() ||
        ny < 0 || ny > current_map->get_height()) {
        handle_collide_wall(dx, dy);
        return false;
    }

    if (current_map->is_collideable(nx, ny, true)) {
        handle_collide_wall(dx, dy);
        return false;
    }

    bool is_collided;
    Entity* e = current_map->check_entity_collision(nx, ny, false);
    current_map->acquire();
    if (e == nullptr) {
        // no collisions so all ok
    } else {
        is_collided = handle_entity_collision(e);
    }
    current_map->release();

    if (!is_collided) {
        player->move_dxdy(dx, dy);
    }

    return is_collided;
}
