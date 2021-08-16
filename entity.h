#ifndef _ENTITY_H
#define _ENTITY_H

#include <stdint.h>

class Entity {
private:
    int x, y, type, texture, state;
    uint32_t id;
public:
    Entity() {
        x = -1; y = -1; type = 0; id = 0; state = 0;
    }
    Entity(int id, int x, int y, int type, int texture) {
        this->x = x;
        this->y = y;
        this->type = type;
        this->texture = texture;
        this->id = id;
    }
    ~Entity() { };

    int get_x() { return x; }
    int get_y() { return y; }
    int get_texture() { return texture; }
    int get_type() { return type; }
    int get_state() { return state; }
    void set_x(int x) { this->x = x; }
    void set_y(int y) { this->y = y; }
    void set_texture(int texture) { this->texture = texture; }
    void set_type(int type) { this->type = type; }
    void set_state(int state) { this->state = state; }

    uint32_t get_id() { return id; }

    void tick_self();
};

bool update_randomwalk_ai(Entity* e, int& dx, int& dy);
bool update_aggressive_ai(Entity* e, int& dx, int& dy);

#endif // _ENTITY_H
