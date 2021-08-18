#include "gamemanager.h"
#include "main.h"
#include "util.h"
#include <iostream>
#include <fstream>

GameManager::GameManager() {
    current_map = nullptr;

    // open startup.txt
    std::ifstream startfile("asset/startup.txt");
    if (!startfile.is_open()) {
        std::cout << "ERROR: unable to open startup.txt" << std::endl;
        exit(1091);
    }

    // fetch a line
    for(std::string line; getline( startfile, line ); ) {
        // tokenize line
        std::vector<std::string> tokens = split_by_char(line, ';');

        if (tokens.size() != 2) {
            std::cout << "ERROR: startup.txt is malformed" << std::endl;
            exit(1092);
        }

        if (tokens[0] == "START_MAP") {
            start_map = tokens[1];
        } else if (tokens[0] == "MAPS_LIST") {
            std::vector<std::string> maps = split_by_char(tokens[1], ',');
            for (std::string& s : maps) {

                Map* m = new Map("asset/" + s);
                if (s == start_map) {
                    current_map = m;
                }
                this->map_table[s] = m;
            }
        }
    }

    if (current_map == nullptr) {
        std::cout << "ERROR: map still not initialized!!" << std::endl;
        exit(1093);
    }

    player->set_x(current_map->get_start_x());
    player->set_y(current_map->get_start_y());

    timer_thread = std::thread(&GameManager::timer_tick, this);
}

GameManager::~GameManager() {

}

void GameManager::edit_timer_state(bool state) {
    std::scoped_lock<std::mutex> lock(timer_mutex);
    is_timer_running = state;
}

void GameManager::alter_game_state(GameState new_state) {
    if (new_state == GameState::NORMAL_PLAY) {
        edit_timer_state(true);
    } else {
        edit_timer_state(false);
    }

    this->game_state = new_state;
}

bool GameManager::timer_tick() {
    while(true) {
        bool should_tick = false;
        {
            std::scoped_lock<std::mutex> lock(timer_mutex);
            if (is_shutdown_ready) {
                break;
            }
            if (is_timer_running) {
                should_tick = true;
                turns_remaining--;
            }
        }
        if (should_tick && current_map != nullptr) {
            current_map->acquire();
            std::vector<Entity*>* entities = current_map->_get_m_entities();
            for (Entity* e : (*entities)) {
                e->tick_self();
            }
            current_map->release();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1250));
        current_map->clean_entity_list();
    }
}

void GameManager::await_shutdown() {
    {
        // stop timer thread
        std::scoped_lock<std::mutex> lock(timer_mutex);
        is_shutdown_ready = true;
    }

    std::cout << "awaiting timer thread..." << std::endl;
    timer_thread.join();
    std::cout << "goodbye." << std::endl;
}

int GameManager::get_turns_remaining() {
    std::scoped_lock<std::mutex> lock(timer_mutex);
    return turns_remaining;
}

GameManager::GameState GameManager::get_game_state() {
    return game_state;
}

void GameManager::mark_portal(Portal* portal) {
    if (portal != nullptr) {
        active_portal = portal;
        has_hit_portal = true;
    }
}

void GameManager::update() {
    if (has_hit_portal) {
        has_hit_portal = false;

        if (map_table.find(active_portal->name) == map_table.end()) {
            // we didnt find the portal
            std::cout << "Erroneous portal trigger: " << active_portal->name << std::endl;
            return;
        }

        current_map = map_table[active_portal->name];
        player->set_x(current_map->get_start_x());
        player->set_y(current_map->get_start_y());
    }
}
