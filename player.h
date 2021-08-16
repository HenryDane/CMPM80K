#ifndef _PLAYER_H
#define _PLAYER_H

#include <stdint.h>

class Player {
private:
    int x, y, hearts, held_item_texture, off_item_texture;
    uint32_t planks_count, coin_count;
public:
    Player(int x, int y, int hearts);
    ~Player();

    int get_x();
    int get_y();
    int get_hearts();
    int get_held_item_texture();
    int get_off_item_texture();
    uint32_t get_planks_count();
    uint32_t get_coin_count();

    void set_x(int x);
    void set_y(int y);
    void set_hearts(int hearts);
    void set_held_item_texture(int t);
    void set_off_item_texture(int t);
    void set_planks_count(uint32_t c);
    void set_coin_count(uint32_t c);

    void move_dxdy(int dx, int dy);
};

#endif // _PLAYER_H
