#ifndef _MAP_H
#define _MAP_H

#include <stdint.h>
#include <string>
#include <vector>

/*
    ENTITY TYPE TABLE
     0 -> invalid
     1 -> chest
     2 -> coin
    12 -> cow
    13 -> pig
    14 -> sheep
    20 -> planks
    21 -> ark
    22 -> enemy
*/

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
};

class Map {
private:
    uint32_t width, height;
    uint8_t* data;
public:
    Map();
    Map(std::string path, std::string entity_path, uint32_t width, uint32_t height);
    ~Map();

    uint32_t get_width();
    uint32_t get_height();
    uint8_t get_tile_at(uint32_t w, uint32_t h);
    void set_tile_at(uint32_t w, uint32_t h, uint8_t tile);
    bool is_collideable(uint32_t w, uint32_t h);
    int get_interaction(uint32_t w, uint32_t h);

    std::vector<Entity> entities;
};

#endif // _MAP_H
