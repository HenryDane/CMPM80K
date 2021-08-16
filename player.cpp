#include "player.h"
#include "main.h"

Player::Player(int x, int y, int hearts) {
    this->x = x;
    this->y = y;
    this->hearts = hearts;
    planks_count = 0;
    coin_count = 0;
    off_item_texture = T_EMPTY;
    held_item_texture = T_EMPTY;
}

int Player::get_x() {
    return x;
}

int Player::get_y() {
    return y;
}

int Player::get_hearts() {
    return hearts;
}

int Player::get_held_item_texture() {
    return held_item_texture;
}

int Player::get_off_item_texture() {
    return off_item_texture;
}

uint32_t Player::get_planks_count() {
    return planks_count;
}

uint32_t Player::get_coin_count() {
    return coin_count;
}

void Player::set_x(int x) {
    this->x = x;
}

void Player::set_y(int y) {
    this->y = y;
}

void Player::set_hearts(int hearts) {
    this->hearts = hearts;
}

void Player::set_held_item_texture(int t) {
    this->held_item_texture = t;
}

void Player::set_off_item_texture(int t) {
    this->off_item_texture = t;
}

void Player::set_planks_count(uint32_t c) {
    this->planks_count = c;
}

void Player::set_coin_count(uint32_t c) {
    this->coin_count = c;
}

void Player::move_dxdy(int dx, int dy) {
    this->x += dx;
    this->y += dy;
}
