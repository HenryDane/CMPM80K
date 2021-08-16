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

struct player_t {
    int x, y, hearts;
    uint32_t sword_type;
    uint32_t piece_count;
    uint32_t gold;
};

extern sf::RenderTexture renderTexture;

extern uint32_t g_next_uuid;

extern int game_state;
extern int turns_remaining;

extern Map* current_map;
extern player_t player;

#endif // _MAIN_H
