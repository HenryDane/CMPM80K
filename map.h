#ifndef _MAP_H
#define _MAP_H

#include <stdint.h>
#include <string>
#include <vector>
#include <mutex>

/*
    ENTITY TYPE TABLE
     0 -> invalid
     1 -> chest
     2 -> coin
     3 -> heart
    12 -> cow
    13 -> pig
    14 -> sheep
    20 -> planks
    21 -> ark
    22 -> enemy
*/
#include "entity.h"

class Portal {
public:
    int x, y;
    std::string name;

    Portal(int x, int y, std::string name) {
        this->x = x;
        this->y = y;
        this->name = name;
    }
};

class Map {
private:
    uint32_t width, height;
    uint8_t* data;
    int startx, starty;
    std::vector<Entity*> entities;
    std::vector<Portal*> portals;
    std::string name;

    std::mutex mutex;
public:
    Map();
    Map(std::string path, std::string entity_path, uint32_t width, uint32_t height);
    Map(std::string path);
    ~Map();

    uint32_t get_width();
    uint32_t get_height();
    int get_start_x();
    int get_start_y();
    std::string get_name();
    uint8_t get_tile_at(uint32_t w, uint32_t h);
    void set_tile_at(uint32_t w, uint32_t h, uint8_t tile);
    bool is_collideable(uint32_t w, uint32_t h, bool _lock = true);
    int get_interaction(uint32_t w, uint32_t h);
    Entity* check_entity_collision(int nx, int ny, bool _lock = true);

    std::vector<Entity*>* _get_m_entities();
    void acquire();
    void release();

    void clean_entity_list();
    void register_entity(Entity* e, bool _lock = true);
};

#endif // _MAP_H
