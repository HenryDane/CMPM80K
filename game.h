#ifndef _GAME_H
#define _GAME_H

#include "main.h"

bool update_player(int x, int y);
void tick_entity(Entity& e);
void do_drop_item();
void do_pickup_item();

#endif // _GAME_H
