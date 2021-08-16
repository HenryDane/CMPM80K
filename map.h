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

class Map {
private:
    friend class Entity; // awful hack to prevent having to lock certian things.....
    uint32_t width, height;
    uint8_t* data;
    std::vector<Entity*> entities;

    std::mutex mutex;
public:
    Map();
    Map(std::string path, std::string entity_path, uint32_t width, uint32_t height);
    ~Map();

    uint32_t get_width();
    uint32_t get_height();
    uint8_t get_tile_at(uint32_t w, uint32_t h);
    void set_tile_at(uint32_t w, uint32_t h, uint8_t tile);
    bool is_collideable(uint32_t w, uint32_t h, bool _lock = true);
    int get_interaction(uint32_t w, uint32_t h);
    //bool check_entity_collision(int nx, int ny, Entity& entity, bool _lock = true);
    Entity* check_entity_collision(int nx, int ny, bool _lock = true);

    std::vector<Entity*>* _get_m_entities();
    void acquire();
    void release();

    void clean_entity_list();
    void register_entity(Entity* e, bool _lock = true);
};

#endif // _MAP_H
