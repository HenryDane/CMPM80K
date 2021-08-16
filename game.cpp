#include "game.h"
#include <iostream>

void handle_collide_wall(int dx, int dy) {
    int nx = player->get_x() + dx;
    int ny = player->get_y() + dy;

    if (current_map->get_tile_at(nx, ny) == T_EMPTY) {
        if (player->get_planks_count() > 0) {
            player->set_planks_count(player->get_planks_count() - 1);
            current_map->set_tile_at(nx, ny, T_PLANKS);
        }
    }
}

void do_drop_item() {
    if (player->get_held_item_texture() < 41 ||
        player->get_held_item_texture() > 43) {
        std::cout << "reset player held item" << std::endl;
        player->set_held_item_texture(T_EMPTY);
        return;
    }

    int dx = 0;
    int dy = 0;

    if (!current_map->is_collideable(player->get_x() + 1, player->get_y())) {
        dx = 1;
    } else if (!current_map->is_collideable(player->get_x() - 1, player->get_y())) {
        dx = -1;
    } else if (!current_map->is_collideable(player->get_x(), player->get_y() + 1)) {
        dy = 1;
    } else if (!current_map->is_collideable(player->get_x(), player->get_y() - 1)) {
        dy = -1;
    } else {
        std::cout << "unable to place entity" << std::endl;
        return;
    }

    Entity* e = new Entity(g_next_uuid++, player->get_x() + dx, player->get_y() + dy,
                           player->get_held_item_texture() - 29, player->get_held_item_texture());
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
        player->set_held_item_texture(entity_n->get_texture());
        entity_n->set_type(-1);
    } else if (entity_s != nullptr) {
        player->set_held_item_texture(entity_s->get_texture());
        entity_s->set_type(-1);
    } else if (entity_e != nullptr) {
        player->set_held_item_texture(entity_e->get_texture());
        entity_e->set_type(-1);
    } else if (entity_w != nullptr) {
        player->set_held_item_texture(entity_w->get_texture());
        entity_w->set_type(-1);
    } else {
        std::cout << "no nearby entities" << std::endl;
        return;
    }
}

bool handle_entity_collision(Entity* e) {
    std::cout << "collided with entity of type " << e << std::endl;

    if (e->get_type() == 2) {
        std::cout << "hit coin" << std::endl;
        player->set_coin_count(player->get_coin_count() + 1);
        e->set_type(-1);
        return false;
    } else if (e->get_type() == 20) {
        std::cout << "hit planks" << std::endl;
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
        std::cout << "hit heart" << std::endl;
        player->set_hearts(player->get_hearts() + 1);
        e->set_type(-1);
        return false;
    } else if (e->get_type() == 22) {
        // TODO make combat more interesting?
        e->set_type(-1);
        std::cout << "enemy vanquished" << std::endl;
        return true;
    } else if (e->get_type() == 21) {
        // collided with the ark
        if (player->get_planks_count() > 0) {
            ark.planks_count += player->get_planks_count();
            player->set_planks_count(0);
        }

        if (player->get_held_item_texture() >= 41 &&
            player->get_held_item_texture() <= 43) {
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
            }

            player->set_held_item_texture(T_EMPTY);
        }

        return true;
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

    if (current_map->is_collideable(nx, ny)) {
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
