#include "entity.h"
#include "main.h"
#include <iostream>
#include <cmath>

void Entity::tick_self() {
    int dx, dy;
    bool valid;

    switch(this->get_type()) {
    case E_COW: // cow
    case E_PIG: // pig
    case E_SHEEP: // sheep
    case E_CHICKEN: // chicken
        valid = update_randomwalk_ai(this, dx, dy);
        break;
    case E_ENEMY0:
    case E_ENEMY1:
    case E_ENEMY2:
        valid = update_aggressive_ai(this, dx, dy);
        break;
    case E_TREE:
        tick_tree(this);
        return;
    default:
        return;
    }

    if (!valid) {
        return;
    }

    int nx = x + dx;
    int ny = y + dy;

    if (current_map->is_collideable(nx, ny, false, false)) {
        return;
    }

    Entity* entity = current_map->check_entity_collision(nx, ny, false);
    if (entity != nullptr) {
        // something got hit by this
        // TODO check if it was the ark and if so, game end
        if (entity->get_type() == E_ARK) {
            std::cout << "enemy hit ark" << std::endl;
            if (ark.cows > 0) {
                ark.cows--;
            } else if (ark.pigs > 0) {
                ark.pigs--;
            } else if (ark.sheep > 0) {
                ark.sheep--;
            } else if (ark.planks_count > 0) {
                ark.planks_count--;
            } else {
                std::cout << "ark is destroyed!!!" << std::endl;
                std::cout << "END THE GAME SOMEHOW??" << std::endl;
            }
        }
        return;
    }

    if (nx == player->get_x() && ny == player->get_y()) {
        // it tried to hit you
        std::cout << "entity hit player" << std::endl;
        if (this->get_type() == E_ENEMY0 ||
            this->get_type() == E_ENEMY1 ||
            this->get_type() == E_ENEMY2) {
            player->set_hearts(player->get_hearts() - 1);
            // TODO setup blink
        }
        return;
    }

    x = nx;
    y = ny;
}

void tick_tree(Entity* e) {
    if (e->get_state() > 0) {
        e->set_state(e->get_state() - 1);
    }

    if (e->get_state() <= 0) {
        e->set_texture(T_TREE);
    } else {
        e->set_texture(T_STUMP);
    }
}

bool update_randomwalk_ai(Entity* e, int& dx, int& dy) {
    int dir = rand() % 4;
    dx = 0;
    dy = 0;

    if (dir == 0) {
        dx = -1;
    } else if (dir == 1) {
        dx = 1;
    } else if (dir == 2) {
        dy = -1;
    } else if (dir == 3) {
        dy = 1;
    }

    // results are valid
    return true;
}

#define ENEMIES_NO_AI
bool update_aggressive_ai(Entity* e, int& dx, int& dy) {
    // calculate direction and clamp it
    // TODO prevent moving diagonally
    // TODO make it move to ark also (if ark closer)
    int dist_ark = sqrt((e->get_x() - ark.x) * (e->get_x() - ark.x) +
                        (e->get_y() - ark.y) * (e->get_y() - ark.y));
    int dist_pla = sqrt((player->get_x() - ark.x) * (player->get_x() - ark.x) +
                        (player->get_y() - ark.y) * (player->get_y() - ark.y));
    if (dist_ark < dist_pla) {
        dx = ark.x - e->get_x();
        dy = ark.y - e->get_y();
    } else {
        dx = player->get_x() - e->get_x();
        dy = player->get_y() - e->get_y();
    }
    if (dx > 1) dx = 1;
    if (dx < -1) dx = -1;
    if (dy > 1) dy = 1;
    if (dy < -1) dy = -1;

    // results are valid;
    return true;
}
