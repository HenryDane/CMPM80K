#include "map.h"
#include "main.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>

Map::Map() {
    width = 64;
    height = 64;
    data = new uint8_t[width * height];
    for (size_t i = 0; i < width * height; i++) {
        data[i] = 0;
    }
}

Map::Map(std::string path, std::string entity_path, uint32_t width, uint32_t height) {
    // TODO make this actually calculate itself from startup
    this->width = width;
    this->height = height;
    data = new uint8_t[width * height];

    // open map file
    std::ifstream mapfile(path);
    if (!mapfile.is_open()) {
        std::cout << "ERROR: unable to open map file: " << path << std::endl;
        exit(1030);
    }

    // parse each line...
    int itr = 0;
    for(std::string line; getline( mapfile, line ); ) {
        // create a string stream and feed it the current line
        std::stringstream ss;
        ss << line;

        // fucking who fucking knows kill me
        while(ss.good()) {
            std::string sstr;
            getline( ss, sstr, ',' );

            data[itr++] = std::stoi(sstr);
        }
    }

    // handle entity file
    int* entity_data = new int[width * height];
    std::ifstream entityfile(entity_path);
    if (!entityfile.is_open()) {
        std::cout << "ERROR: unable to open map file: " << path << std::endl;
    } else {
        // parse each line...
        itr = 0;
        for(std::string line; getline( entityfile, line ); ) {
            // create a string stream and feed it the current line
            std::stringstream ss;
            ss << line;

            // fucking who fucking knows kill me
            while(ss.good()) {
                std::string sstr;
                getline( ss, sstr, ',' );

                entity_data[itr++] = std::stoi(sstr);
            }
        }
    }

    // process entity data
    for (int y = 0; y < this->height; y++) {
        for (int x = 0; x < this->width; x++) {
            int type = entity_data[y * this->width + x];
            if (type >= 41 && type <= 43) {
                // animals
                Entity e(g_next_uuid++, x, y, type - 29, type);
                entities.push_back(e);
            } else if (type == 62) {
                // planks
                Entity e(g_next_uuid++, x, y, 20, type);
                entities.push_back(e);
            } else if (type == 61) {
                // chest
                Entity e(g_next_uuid++, x, y, 1, type);
                entities.push_back(e);
            } else if (type == 60) {
                // coin
                Entity e(g_next_uuid++, x, y, 2, type);
                entities.push_back(e);
            } else if (type == 63) {
                // coin
                Entity e(g_next_uuid++, x, y, 22, type);
                entities.push_back(e);
            }
        }
    }

    // free entity data
    delete[] entity_data;
}

Map::~Map() {
    delete[] this->data;
}

uint32_t Map::get_width() {
    return width;
}

uint32_t Map::get_height() {
    return height;
}

uint8_t Map::get_tile_at(uint32_t x, uint32_t y) {
    if (y >= height || x >= width) {
        return 0;
    }
    return data[y * width + x];
}

void Map::set_tile_at(uint32_t x, uint32_t y, uint8_t tile) {
    if (y >= height || x >= width) {
        return;
    }
    data[y * width + x] = tile;
}

bool Map::is_collideable(uint32_t x, uint32_t y) {
    if (y >= height || x >= width) {
        return true;
    }

    uint8_t t = data[y * width + x];
    if (t % 11 < 5) {
        return true;
    }

    return false;
}

int Map::get_interaction(uint32_t x, uint32_t y) {
    if (y >= height || x >= width) {
        return 0;
    }

    // TODO make this work correctly
    return 0;
}
