#include "entity.h"
#include "main.h"
#include <iostream>

void Entity::tick_self() {
    int dx, dy;
    bool valid;

    switch(this->get_type()) {
    case 12: // cow
    case 13: // pig
    case 14: // sheep
        valid = update_randomwalk_ai(this, dx, dy);
        break;
    case 22:
        valid = update_aggressive_ai(this, dx, dy);
        break;
    default:
        return;
        break;
    }

    if (!valid) {
        return;
    }

    int nx = x + dx;
    int ny = y + dy;

    if (current_map->is_collideable(nx, ny, false)) {
        return;
    }

    Entity* entity = current_map->check_entity_collision(nx, ny, false);
    if (entity != nullptr) {
        // something got hit by this
        // TODO check if it was the ark and if so, game end
        return;
    }

    if (nx == player->get_x() && ny == player->get_y()) {
        // it tried to hit you
        std::cout << "entity hit player" << std::endl;
        if (this->get_type() == 22) {
            player->set_hearts(player->get_hearts() - 1);
        }
        return;
    }

    x = nx;
    y = ny;
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

bool update_aggressive_ai(Entity* e, int& dx, int& dy) {
    // calculate direction and clamp it
    // TODO prevent moving diagonally
    // TODO make it move to ark also (if ark closer)
    dx = player->get_x() - e->get_x();
    dy = player->get_y() - e->get_y();
    if (dx > 1) dx = 1;
    if (dx < -1) dx = -1;
    if (dy > 1) dy = 1;
    if (dy < -1) dy = -1;

    // results are valid;
    return true;
}
