#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H

#include <mutex>
#include <thread>
#include <map>
#include "map.h"

class GameManager {
public:
    enum GameState {
        UNINITIALIZED=0, STARTING,
        MAIN_MENU=2, CREDITS=3, OPTIONS=4,
        NORMAL_PLAY=10, PAUSED=50,
        OPEN_CUTSCENE=13, WIN_CUTSCENE=11, LOOSE_CUTSCENE=12,
        //SAVE=51, LOAD=52,
        CONFIRM_SAVE = 51, CONFIRM_QUIT = 52, DIALOGUE=53,
    };

    GameManager();
    ~GameManager();

    void alter_game_state(GameState new_state);
    bool timer_tick();
    void await_shutdown();
    int get_turns_remaining();
    GameState get_game_state();

    void mark_portal(Portal* portal);
    void update();

    void initalize_map_data();
    void destroy_map_data();
private:
    // timer and thread information
    bool is_timer_running = false;
    bool is_shutdown_ready = false;
    std::mutex timer_mutex;
    int turns_remaining = 500;
    std::thread timer_thread;

    // map data
    std::string start_map;
    std::map<std::string, Map*> map_table;

    // portal info
    bool has_hit_portal = false;
    Portal* active_portal = nullptr;

    // state information
    GameState game_state = GameState::UNINITIALIZED;

    void edit_timer_state(bool state);
};

#endif // _GAMEMANAGER_H
