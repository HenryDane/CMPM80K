#include "game.h"
#include <iostream>

void handle_collide_wall(int dx, int dy) {
    //std::cout << "collided with wall" << std::endl;
}

bool handle_entity_collision(Entity& e) {
    //std::cout << "collided with entity of type " << e.get_type() << std::endl;

    return false;
}

bool update_player(int dx, int dy) {
    int nx = player.x + dx;
    int ny = player.y + dy;

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
    Entity e;
    if (!current_map->check_entity_collision(nx, ny, e)) {
        // no collisions so all ok
    } else {
        is_collided = handle_entity_collision(e);
    }

    if (!is_collided) {
        player.x += dx;
        player.y += dy;
    }

    return is_collided;
}
