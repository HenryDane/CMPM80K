#include "game.h"
#include <iostream>

void handle_collide_wall(int dx, int dy) {
    std::cout << "collided with wall" << std::endl;
}

bool handle_entity_collision(Entity& e) {
    std::cout << "collided with entity of type " << e.get_type() << std::endl;

    return false;
}

bool update_player(int dx, int dy) {
    int nx = player.x + dx;
    int ny = player.y + dy;

    if (nx < 0 || nx > current_map->get_width() ||
        ny < 0 || ny > current_map->get_height()) {
        handle_collide_wall(dx, dy);
        return false;
    }

    if (current_map->is_collideable(nx, ny)) {
        handle_collide_wall(dx, dy);
        return false;
    }

    bool is_collided = false;
    std::vector<Entity> collided;
    for (Entity& e : current_map->entities) {
        if (e.get_x() == nx && e.get_y() == ny) {
            if (handle_entity_collision(e)) {
                is_collided = true;
            }
        }
    }

    if (!is_collided) {
        player.x += dx;
        player.y += dy;
    }
}
