#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H

#include <mutex>
#include <thread>

class GameManager {
public:
    enum GameState {
        UNINITIALIZED=0, STARTING,
        MAIN_MENU=2, CREDITS=3, OPTIONS=4,
        NORMAL_PLAY=10, PAUSED=50,
        OPEN_CUTSCENE=13, WIN_CUTSCENE=11, LOOSE_CUTSCENE=12,
        //SAVE=51, LOAD=52,
        CONFIRM_SAVE = 51, CONFIRM_QUIT = 52
    };
private:
    // timer and thread information
    bool is_timer_running = false;
    bool is_shutdown_ready = false;
    std::mutex timer_mutex;
    int turns_remaining = 500;
    std::thread timer_thread;

    // state information
    GameState game_state = GameState::UNINITIALIZED;

    void edit_timer_state(bool state);
public:
    GameManager();
    ~GameManager();

    void alter_game_state(GameState new_state);
    bool timer_tick();
    void await_shutdown();
    int get_turns_remaining();
    GameState get_game_state();
};

#endif // _GAMEMANAGER_H
