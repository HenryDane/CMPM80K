#include "map.h"
#include "main.h"
#include "util.h"
#include "draw.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>

Map::Map() {
    width = 64;
    height = 64;
    data = new uint16_t[width * height];
    for (size_t i = 0; i < width * height; i++) {
        data[i] = 0;
    }
}

Map::Map(std::string& path) {
    // first, lock this
    std::scoped_lock<std::mutex> lock(mutex);

    // open .map file
    std::ifstream mapfile(path);
    if (!mapfile.is_open()) {
        std::cout << "ERROR: unable to open map file: " << path << std::endl;
        std::cerr << "Error: " << std::strerror(errno);
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
    uint16_t* entity_data;

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
                this->data = new uint16_t[width * height];
                this->decoration = new uint16_t[width * height];
                for (size_t i = 0; i < width*height; decoration[i++] = -1);
                entity_data = new uint16_t[width * height];
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
                    uint16_t d = (uint16_t) std::stoi(values[i]);
                    data[i] = d - 1;
                }
            }
        } else if (tokens[0] == "DECORATION") {
            std::vector<std::string> values = split_by_char(tokens[1], ',');
            if (values.size() != width * height) {
                std::cout << "TILES ENTRY HAS INCORRECT LENGTH: " << path << std::endl;
                exit(1034);
            } else {
                for (size_t i = 0; i < values.size(); i++) {
                    uint16_t d = (uint16_t) std::stoi(values[i]);
                    decoration[i] = d - 1;
                }
            }
        } else if (tokens[0] == "ENTITIES") {
            std::vector<std::string> values = split_by_char(tokens[1], ',');
            if (values.size() != width * height) {
                std::cout << "ENTITIES ENTRY HAS INCORRECT LENGTH: " << path << std::endl;
                exit(1034);
            } else {
                for (size_t i = 0; i < values.size(); i++) {
                    uint16_t e = (uint16_t) std::stoi(values[i]);
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
            if (dimtokens.size() == 4) {
                this->portals.push_back(new Portal(std::stoi(dimtokens[0]) / 16,
                                                   std::stoi(dimtokens[1]) / 16,
                                                   dimtokens[2],
                                                   std::stoi(dimtokens[3])));
            } else {
                std::cout << "MALFORMED PORTAL TOKEN IN FILE: " << path << std::endl;
                exit(1033);
            }
        } else if (tokens[0] == "DOOR") {
            std::vector<std::string> dimtokens = split_by_char(tokens[1], ',');
            if (dimtokens.size() == 5) {
                this->portals.push_back(new Portal(std::stoi(dimtokens[0]) / 16,
                                                   std::stoi(dimtokens[1]) / 16,
                                                   dimtokens[2],
                                                   std::stoi(dimtokens[3]),
                                                   std::stoi(dimtokens[4])));
            } else {
                std::cout << "MALFORMED PORTAL TOKEN IN FILE: " << path << std::endl;
                exit(1033);
            }
        }else if (tokens[0] == "DIALOGUE") {
            std::vector<std::string> dimtokens = split_by_char(tokens[1], ',');
            if (dimtokens.size() >= 4) {
                int x = std::stoi(dimtokens[0]) / 16;
                int y = std::stoi(dimtokens[1]) / 16;
                std::string name = dimtokens[2];
                int n = std::stoi(dimtokens[3]);

                if (dimtokens.size() != 4 + n) {
                    std::cout << "MALFORMED DIALOGUE COUNT IN FILE: " << path << std::endl;
                    exit(1033);
                }

                Dialogue* d = new Dialogue(x, y, name);
                for (int i = 4; i < 4 + n; i++) {
                    d->add_text(dimtokens[i]);
                }
                this->dialogue.push_back(d);
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
                // enemy
                Entity* e = new Entity(g_next_uuid++, x, y, E_ENEMY0, type);
                e->set_state(4);
                entities.push_back(e);
            } else if (type == 94) {
                // enemy
                Entity* e = new Entity(g_next_uuid++, x, y, E_ENEMY1, type);
                e->set_state(6);
                entities.push_back(e);
            }else if (type == 95) {
                // enemy
                Entity* e = new Entity(g_next_uuid++, x, y, E_ENEMY2, type);
                e->set_state(9);
                entities.push_back(e);
            }else if (type == 64) {
                if (ark->on_map) {
                    // there can only BE ONE!!!
                    continue;
                }
                // ark
                Entity* e = new Entity(g_next_uuid++, x, y, 21, type);
                ark->x = x;
                ark->y = y;
                ark->on_map = true;
                entities.push_back(e);
            } else if (type == 52) {
                // heart
                Entity* e = new Entity(g_next_uuid++, x, y, 3, type);
                entities.push_back(e);
            } else if (type == 65) {
                // heart
                Entity* e = new Entity(g_next_uuid++, x, y, 15, type);
                entities.push_back(e);
            } else if (type == 92) {
                // tree
                Entity* e = new Entity(g_next_uuid++, x, y, 30, type);
                entities.push_back(e);
            } else if (type == 71) {
                // heart
                Entity* e = new Entity(g_next_uuid++, x, y, 40, type);
                entities.push_back(e);
            } else if (type == 72) {
                // heart
                Entity* e = new Entity(g_next_uuid++, x, y, 41, type);
                entities.push_back(e);
            } else if (type == 73) {
                // heart
                Entity* e = new Entity(g_next_uuid++, x, y, 42, type);
                entities.push_back(e);
            } else if (type == 74) {
                // heart
                Entity* e = new Entity(g_next_uuid++, x, y, 43, type);
                entities.push_back(e);
            }
        }
    }

    delete[] entity_data;
}

Map::~Map() {
    std::scoped_lock<std::mutex> lock(mutex);
    delete[] this->data;
    delete[] this->decoration;
    for (size_t i = 0; i < entities.size(); i++) {
        delete entities[i];
    }
    for (size_t i = 0; i < portals.size(); i++) {
        delete portals[i];
    }
    for (int i = 0; i < dialogue.size(); i++) {
        delete dialogue[i];
    }
}

uint32_t Map::get_width() {
    std::scoped_lock<std::mutex> lock(mutex);
    return width;
}

uint32_t Map::get_height() {
    std::scoped_lock<std::mutex> lock(mutex);
    return height;
}

uint16_t Map::get_tile_at(uint32_t x, uint32_t y) {
    std::scoped_lock<std::mutex> lock(mutex);
    if (y >= height || x >= width) {
        return 0;
    }
    return data[y * width + x];
}

uint16_t Map::get_decor_at(uint32_t x, uint32_t y) {
    std::scoped_lock<std::mutex> lock(mutex);
    if (y >= height || x >= width) {
        return 0;
    }
    return decoration[y * width + x];
}

void Map::set_tile_at(uint32_t x, uint32_t y, uint16_t tile) {
    std::scoped_lock<std::mutex> lock(mutex);
    if (y >= height || x >= width) {
        return;
    }
    data[y * width + x] = tile;
}

bool Map::is_collideable(uint32_t x, uint32_t y, bool is_player, bool _lock) {
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

    uint16_t t = data[y * width + x];
    if (t < 48) {
        if (t % 11 < 5) {
            if (_lock) {
                mutex.unlock();
            }
            return true;
        }
    } else if (t == 119 || t == 185 || t == 218) {
        // do nothing
    } else if (t == 259 || t == 260 || t == 270 || t == 271 || t == 287) {
        if (_lock) {
            mutex.unlock();
        }
        return true;
    } else if (t >= 99 && t <= 109) {
        if (_lock) {
            mutex.unlock();
        }
        return true;
    } else if ((t >= 110 && t <= 131) ||
               (t >= 165 && t <= 285)) {
        if (t % 11 > 7) {
            if (_lock) {
                mutex.unlock();
            }
            return true;
        }
    } else if (t >= 286) {
        if (t % 11 > 1) {
            if (_lock) {
                mutex.unlock();
            }
            return true;
        }
    }

    // check portals and dialogue last
    for (Portal* p : portals) {
        if (p->x == x && p->y == y) {
            // hit a portal
            if (is_player) {
                game->mark_portal(p);
                std::cout << "hit portal: " << p << " with door flag: " << p->is_door << std::endl;
                if (p->tutorial_end) {
                    ark->exists = true;
                    delete player;
                    player = new Player(0, 0, 2);
                }
                if (_lock) {
                    mutex.unlock();
                }
                // its a player so it can stand here but were also gonna activate the portal
                return false;
            } else {
                if (_lock) {
                    mutex.unlock();
                }
                // its not a player so this is basically a wall
                return true;
            }
        }
    }
    for (Dialogue* d : dialogue) {
        if (d->x == x && d->y == y) {
            if (is_player) {
                active_dialogue = d;
                dialogue_state = 0;
                game->alter_game_state(GameManager::DIALOGUE);
                if (_lock) {
                    mutex.unlock();
                }
                return false;
            } else {
                if (_lock) {
                    mutex.unlock();
                }
                return true;
            }
        }
    }

    if (_lock) {
        mutex.unlock();
    }
    return false;
}

void Map::tick_water() {
    std::scoped_lock<std::mutex> lock(mutex);

    for (size_t i = 0; i < width * height; i++) {
        if (data[i] == 286) {
            data[i] = 287;
        } else if (data[i] == 287) {
            data[i] = 286;
        }
        if (decoration[i] == 137) {
            decoration[i] = 148;
        } else if (decoration[i] == 148) {
            decoration[i] = 137;
        }
    }
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

Dialogue::Dialogue(int x, int y, std::string name) {
    this->x = x;
    this->y = y;
    this->name = name;
}

Dialogue::~Dialogue() {
//    for(int i = 0; i < text.size(); i++) {
//        delete text[i];
//        text[i] = nullptr;
//    }
}

void Dialogue::add_text(std::string str) {
    sf::Text t;
    t.setFont(font);
    t.setCharacterSize(14);
    t.setFillColor(sf::Color(255,255,255));
    t.setString(str);
    t.setPosition(64+8,480-(8*16)+8);

    sf::FloatRect fr = t.getLocalBounds();
    if (fr.width <= 496) {
        text.push_back(t);
        return;
    }


    // iterate over every character in the string
    // check if the current character is out of bounds
    // if so, go back to prev space and insert a newline
    // if the character is a space, save it to most recent
    int most_recent_space = 0;
    for (auto i = 0u; i < t.getString().getSize(); i++) {
        if (t.findCharacterPos(i).x > 512) {
//            str.insert(most_recent_space, "\n");
            str[most_recent_space] = '\n';
            t.setString(str);
        }
        if (str.at(i) == ' ') {
            most_recent_space = i;
        }

    }
    text.push_back(t);
}
