#include "gamemanager.h"
#include "main.h"
#include <iostream>

GameManager::GameManager() {
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
        if (should_tick) {
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
