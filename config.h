#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdint.h>
#include <string>

class Config {
private:
    uint32_t map_w, map_h;
    int start_x, start_y;
    std::string map_id_ground;
    std::string map_id_entity;

public:
    Config();
    ~Config();

    uint32_t get_map_w() { return map_w; }
    uint32_t get_map_h() { return map_h; }
    std::string get_map_id_ground() { return map_id_ground; }
    std::string get_map_id_entity() { return map_id_entity; }

    uint32_t get_player_sx() { return start_x; }
    uint32_t get_player_sy() { return start_y; }
};

#endif // _CONFIG_H
