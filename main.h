#ifndef _MAIN_H
#define _MAIN_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <stdint.h>
#include "map.h"
#include "player.h"
#include "gamemanager.h"

#define SCREEN_W      640
#define SCREEN_H      480
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
#define TILE_WIDTH     16
#define TILE_HEIGHT    16
#define TEX_SCALE       1

#define T_PLAYER   18
#define T_GRASS    8
#define T_BRICK    15
#define T_CHEST    61
#define T_COIN     60
#define T_HEART    52
#define T_EMPTY    35
#define T_KEY_A    82
#define T_KEY_Z    86
#define T_KEY_X    87
#define T_KEY_C    56
#define T_PLANK    62
#define T_PLANKS   5
#define T_RIGHT    75
#define T_DOWN     76
#define T_SAND     53
#define T_TREE     92
#define T_TREE_TOP 93
#define T_STUMP    37
#define T_AXE      71
#define T_SWORD0   72
#define T_SWORD1   73
#define T_SWORD2   74

struct ark_t {
    int planks_count;
    int cows;
    int sheep;
    int pigs;

    int x, y;
    bool on_map;
};

extern sf::RenderTexture renderTexture;
extern sf::RenderWindow* renderWindow;

extern uint32_t g_next_uuid;

extern Map* current_map;
extern Player* player;
extern ark_t ark;
extern GameManager* game;
extern Dialogue* active_dialogue;
extern int dialogue_state;

extern int current_menu_sel;

extern sf::Clock input_timer;

#endif // _MAIN_H
