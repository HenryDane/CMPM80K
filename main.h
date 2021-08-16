#ifndef _MAIN_H
#define _MAIN_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <stdint.h>
#include "map.h"

#define SCREEN_W 640
#define SCREEN_H 480
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define TILE_WIDTH 16
#define TILE_HEIGHT 16
#define TEX_SCALE 1

#define T_PLAYER 18
#define T_GRASS  8
#define T_BRICK  15
#define T_CHEST  61
#define T_COIN   60
#define T_HEART  52
#define T_EMPTY  35
#define T_KEY_A  44
#define T_KEY_Z  45
#define T_KEY_X  55
#define T_KEY_C  56

struct player_t {
    int x, y, hearts, held_item_texture, off_item_texture;
    uint32_t planks_count;
    uint32_t coin_count;
};

extern sf::RenderTexture renderTexture;
extern sf::RenderWindow* renderWindow;

extern uint32_t g_next_uuid;

extern int game_state;
extern int turns_remaining;

extern Map* current_map;
extern player_t player;

extern int current_menu_sel;

#endif // _MAIN_H
