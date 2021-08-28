#ifndef _MAIN_H
#define _MAIN_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <stdint.h>
#include "map.h"
#include "ark.h"
#include "player.h"
#include "gamemanager.h"

#define SCREEN_W      640
#define SCREEN_H      480
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
#define TILE_WIDTH     32
#define TILE_HEIGHT    32
#define TEX_SCALE       2

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
#define T_KEY_W    85
#define T_KEY_S    83
#define T_KEY_J    297
#define T_KEY_K    298
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
#define T_PIG      42
#define T_COW      41
#define T_SHEEP    43
#define T_CHICKEN  65
#define T_ARK0     96
#define T_ARK1     36
#define T_ARK2     44
#define T_ARK3     46
#define T_PLAYERA  304
#define T_PLAYER0  305
#define T_PLAYER1  306
#define T_PLAYER2  307

extern sf::RenderTexture renderTexture;
//extern sf::RenderWindow* renderWindow;

extern uint32_t g_next_uuid;

extern Map* current_map;
extern Player* player;
extern Ark* ark;
extern GameManager* game;
extern Dialogue* active_dialogue;
extern int dialogue_state;

extern int current_menu_sel;

extern sf::Clock input_timer;

#endif // _MAIN_H
