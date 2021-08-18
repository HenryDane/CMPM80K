#include "map.h"
#include "main.h"
#include "util.h"
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

Map::Map(std::string path) {
    // first, lock this
    std::scoped_lock<std::mutex> lock(mutex);

    // open .map file
    std::ifstream mapfile(path);
    if (!mapfile.is_open()) {
        std::cout << "ERROR: unable to open map file: " << path << std::endl;
        exit(1031);
    }

    // setup values
    width = 0;
    height = 0;
    data = nullptr;
    startx = -1;
    starty = -1;
    entities.clear();

    // temporary entity data
    uint8_t* entity_data;

    // fetch a line
    for(std::string line; getline( mapfile, line ); ) {
        // tokenize line
        std::vector<std::string> tokens = split_by_char(line, ';');

        if (tokens.size() != 2) {
            std::cout << "MALFORMED TOKEN IN FILE: " << path << std::endl;
            exit(1032);
        }

        if (tokens[0] == "NAME") {
            this->name = tokens[1];
        } else if (tokens[0] == "MAPDIM") {
            std::vector<std::string> dimtokens = split_by_char(tokens[1], ',');
            if (dimtokens.size() == 2) {
                this->width = std::stoi(dimtokens[0]);
                this->height = std::stoi(dimtokens[1]);
                this->data = new uint8_t[width * height];
                entity_data = new uint8_t[width * height];
            } else {
                std::cout << "MALFORMED MAPDIM TOKEN IN FILE: " << path << std::endl;
                exit(1033);
            }
        } else if (tokens[0] == "TILES") {
            std::vector<std::string> values = split_by_char(tokens[1], ',');
            if (values.size() != width * height) {
                std::cout << "TILES ENTRY HAS INCORRECT LENGTH: " << path << std::endl;
                exit(1034);
            } else {
                for (size_t i = 0; i < values.size(); i++) {
                    uint8_t d = (uint8_t) std::stoi(values[i]);
                    data[i] = d - 1;
                }
            }
        } else if (tokens[0] == "ENTITIES") {
            std::vector<std::string> values = split_by_char(tokens[1], ',');
            if (values.size() != width * height) {
                std::cout << "ENTITIES ENTRY HAS INCORRECT LENGTH: " << path << std::endl;
                exit(1034);
            } else {
                for (size_t i = 0; i < values.size(); i++) {
                    uint8_t e = (uint8_t) std::stoi(values[i]);
                    entity_data[i] = e - 1;
                }
            }
        } else if (tokens[0] == "START") {
            std::vector<std::string> dimtokens = split_by_char(tokens[1], ',');
            if (dimtokens.size() == 2) {
                this->startx = std::stoi(dimtokens[0]);
                this->starty = std::stoi(dimtokens[1]);
            } else {
                std::cout << "MALFORMED START TOKEN IN FILE: " << path << std::endl;
                exit(1033);
            }
        } else if (tokens[0] == "PORTAL") {
            std::vector<std::string> dimtokens = split_by_char(tokens[1], ',');
            if (dimtokens.size() == 3) {
                this->portals.push_back(new Portal(std::stoi(dimtokens[0]), std::stoi(dimtokens[1]), dimtokens[2]));
            } else {
                std::cout << "MALFORMED START TOKEN IN FILE: " << path << std::endl;
                exit(1033);
            }
        }
    }

    // process entity data
    for (uint32_t y = 0; y < this->height; y++) {
        for (uint32_t x = 0; x < this->width; x++) {
            int type = entity_data[y * this->width + x];
            if (type >= 41 && type <= 43) {
                // animals
                Entity* e = new Entity(g_next_uuid++, x, y, type - 29, type);
                entities.push_back(e);
            } else if (type == 62) {
                // planks
                Entity* e = new Entity(g_next_uuid++, x, y, 20, type);
                entities.push_back(e);
            } else if (type == 61) {
                // chest
                Entity* e = new Entity(g_next_uuid++, x, y, 1, type);
                entities.push_back(e);
            } else if (type == 60) {
                // coin
                Entity* e = new Entity(g_next_uuid++, x, y, 2, type);
                entities.push_back(e);
            } else if (type == 63) {
                // coin
                Entity* e = new Entity(g_next_uuid++, x, y, 22, type);
                entities.push_back(e);
            } else if (type == 64) {
                if (ark.on_map) {
                    // there can only BE ONE!!!
                    continue;
                }
                // ark
                Entity* e = new Entity(g_next_uuid++, x, y, 21, type);
                ark.x = x;
                ark.y = y;
                ark.on_map = true;
                entities.push_back(e);
            }
        }
    }

    delete[] entity_data;
}

Map::Map(std::string path, std::string entity_path, uint32_t width, uint32_t height) {
    std::scoped_lock<std::mutex> lock(mutex);

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
    for (uint32_t y = 0; y < this->height; y++) {
        for (uint32_t x = 0; x < this->width; x++) {
            int type = entity_data[y * this->width + x];
            if (type >= 41 && type <= 43) {
                // animals
                Entity* e = new Entity(g_next_uuid++, x, y, type - 29, type);
                entities.push_back(e);
            } else if (type == 62) {
                // planks
                Entity* e = new Entity(g_next_uuid++, x, y, 20, type);
                entities.push_back(e);
            } else if (type == 61) {
                // chest
                Entity* e = new Entity(g_next_uuid++, x, y, 1, type);
                entities.push_back(e);
            } else if (type == 60) {
                // coin
                Entity* e = new Entity(g_next_uuid++, x, y, 2, type);
                entities.push_back(e);
            } else if (type == 63) {
                // coin
                Entity* e = new Entity(g_next_uuid++, x, y, 22, type);
                entities.push_back(e);
            } else if (type == 64) {
                if (ark.on_map) {
                    // there can only BE ONE!!!
                    continue;
                }
                // ark
                Entity* e = new Entity(g_next_uuid++, x, y, 21, type);
                ark.x = x;
                ark.y = y;
                ark.on_map = true;
                entities.push_back(e);
            }
        }
    }

    // free entity data
    delete[] entity_data;
}

Map::~Map() {
    std::scoped_lock<std::mutex> lock(mutex);
    delete[] this->data;
}

uint32_t Map::get_width() {
    std::scoped_lock<std::mutex> lock(mutex);
    return width;
}

uint32_t Map::get_height() {
    std::scoped_lock<std::mutex> lock(mutex);
    return height;
}

uint8_t Map::get_tile_at(uint32_t x, uint32_t y) {
    std::scoped_lock<std::mutex> lock(mutex);
    if (y >= height || x >= width) {
        return 0;
    }
    return data[y * width + x];
}

void Map::set_tile_at(uint32_t x, uint32_t y, uint8_t tile) {
    std::scoped_lock<std::mutex> lock(mutex);
    if (y >= height || x >= width) {
        return;
    }
    data[y * width + x] = tile;
}

bool Map::is_collideable(uint32_t x, uint32_t y, bool _lock) {
    //std::scoped_lock<std::mutex> lock(mutex);
    if (_lock) {
        mutex.lock();
    }

    if (y >= height || x >= width) {
        if (_lock) {
            mutex.unlock();
        }
        return true;
    }

    uint8_t t = data[y * width + x];
    if (t % 11 < 5) {
        if (_lock) {
            mutex.unlock();
        }
        return true;
    }

    if (_lock) {
        mutex.unlock();
    }
    return false;
}

int Map::get_interaction(uint32_t x, uint32_t y) {
    std::scoped_lock<std::mutex> lock(mutex);
    if (y >= height || x >= width) {
        return 0;
    }

    // TODO make this work correctly
    return 0;
}

// returns true if valid
Entity* Map::check_entity_collision(int nx, int ny, bool _lock) {
    //std::scoped_lock<std::mutex> lock(mutex);
    if (_lock) {
        mutex.lock();
    }

    for (Entity* e : entities) {
        if (e->get_x() == nx && e->get_y() == ny && e->get_type() > 0) {
            if (_lock) {
                mutex.unlock();
            }
            return e;
        }
    }

    if (_lock) {
        mutex.unlock();
    }
    return nullptr;
}

void Map::clean_entity_list() {
    std::scoped_lock<std::mutex> lock(mutex);
    for (Entity* e : entities) {
        if (e == nullptr) {
            continue;
        }
        if (e->get_type() < 0) {
            // TODO: fix this mess
            //delete e;
            e = nullptr;
        }
    }
    entities.erase(std::remove(begin(entities), end(entities), nullptr),
             end(entities));
}

void Map::register_entity(Entity* e, bool _lock) {
    if (_lock) {
        mutex.lock();
    }

    entities.push_back(e);

    if (_lock) {
        mutex.unlock();
    }
}

void Map::acquire() {
    mutex.lock();
}

std::vector<Entity*>* Map::_get_m_entities() {
    return &entities;
}

void Map::release() {
    mutex.unlock();
}

int Map::get_start_x() {
    return startx;
}

int Map::get_start_y() {
    return starty;
}

std::string Map::get_name() {
    return name;
}

