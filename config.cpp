#include "config.h"

#include <fstream>
#include <string>
#include <sstream>

Config::Config() {
    // TODO make this actually useful
    map_w = 40;
    map_h = 30;
    start_x = 5;
    start_y = 2;
    map_id_ground = "asset/test_map_Ground.csv";
    map_id_entity = "asset/test_map_Entities.csv";
}

Config::~Config() {

}
