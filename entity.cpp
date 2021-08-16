#include "entity.h"
#include "main.h"

void Entity::tick_self() {
    switch(this->get_type()) {
    case 12: // cow
    case 13: // pig
    case 14: // sheep
        update_randomwalk_ai(this);
        break;
    case 22:
        update_aggressive_ai(this);
        break;
    default:
        break;
    }
}

void update_randomwalk_ai(Entity* e) {
    int dir = rand() % 4;
    int dx = 0;
    int dy = 0;

    if (dir == 0) {
        dx = -1;
    } else if (dir == 1) {
        dx = 1;
    } else if (dir == 2) {
        dy = -1;
    } else if (dir == 3) {
        dy = 1;
    }

    if (current_map->is_collideable(e->get_x() + dx, e->get_y() + dy, false)) {
        return;
    }

    Entity entity;
    if (current_map->check_entity_collision(e->get_x() + dx, e->get_y() + dy, entity, false)) {
        return;
    }

    e->set_x(e->get_x() + dx);
    e->set_y(e->get_y() + dy);
}

void update_aggressive_ai(Entity* e) {

}
