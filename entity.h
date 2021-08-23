#ifndef _ENTITY_H
#define _ENTITY_H

#include <stdint.h>

/*
    ENTITY TYPE TABLE
     0 -> invalid
     1 -> chest
     2 -> coin
     3 -> heart
    12 -> cow
    13 -> pig
    14 -> sheep
    15 -> chicken
    20 -> planks
    21 -> ark
    22 -> enemy
    30 -> tree
    40 -> axe
*/

#define E_ARK      21
#define E_ENEMY0   22
#define E_ENEMY1   23
#define E_ENEMY2   24
#define E_COW      12
#define E_PIG      13
#define E_SHEEP    14
#define E_CHICKEN  15
#define E_PLANKS   20
#define E_AXE      40
#define E_SWORD0   41
#define E_SWORD1   42
#define E_SWORD2   43
#define E_TREE     30

class Entity {
private:
    int x, y, type, texture, state, blink;
    uint32_t id;
public:
    Entity() {
        x = -1; y = -1; type = 0; id = 0; state = 0; blink = 0;
    }
    Entity(int id, int x, int y, int type, int texture) {
        this->x = x;
        this->y = y;
        this->type = type;
        this->texture = texture;
        this->id = id;
        this->state = 0;
        this->blink = 0;
    }
    ~Entity() { };

    int get_x() { return x; }
    int get_y() { return y; }
    int get_texture() { return texture; }
    int get_type() { return type; }
    int get_state() { return state; }
    int get_blink() { return blink; }
    void set_x(int x) { this->x = x; }
    void set_y(int y) { this->y = y; }
    void set_texture(int texture) { this->texture = texture; }
    void set_type(int type) { this->type = type; }
    void set_state(int state) { this->state = state; }
    void set_blink(int blink) { this->blink = blink; }

    uint32_t get_id() { return id; }

    void tick_self();
};

bool update_randomwalk_ai(Entity* e, int& dx, int& dy);
bool update_aggressive_ai(Entity* e, int& dx, int& dy);
void tick_tree(Entity* e);

#endif // _ENTITY_H
