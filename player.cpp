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
    std::scoped_lock<std::mutex> lock(this->mutex);
    return x;
}

int Player::get_y() {
    std::scoped_lock<std::mutex> lock(this->mutex);
    return y;
}

int Player::get_hearts() {
    std::scoped_lock<std::mutex> lock(this->mutex);
    return hearts;
}

int Player::get_held_item_texture() {
    std::scoped_lock<std::mutex> lock(this->mutex);
    return held_item_texture;
}

int Player::get_off_item_texture() {
    std::scoped_lock<std::mutex> lock(this->mutex);
    return off_item_texture;
}

uint32_t Player::get_planks_count() {
    std::scoped_lock<std::mutex> lock(this->mutex);
    return planks_count;
}

uint32_t Player::get_coin_count() {
    std::scoped_lock<std::mutex> lock(this->mutex);
    return coin_count;
}

void Player::set_x(int x) {
    std::scoped_lock<std::mutex> lock(this->mutex);
    this->x = x;
}

void Player::set_y(int y) {
    std::scoped_lock<std::mutex> lock(this->mutex);
    this->y = y;
}

void Player::set_hearts(int hearts) {
    std::scoped_lock<std::mutex> lock(this->mutex);
    this->hearts = hearts;
}

void Player::set_held_item_texture(int t) {
    std::scoped_lock<std::mutex> lock(this->mutex);
    this->held_item_texture = t;
}

void Player::set_off_item_texture(int t) {
    std::scoped_lock<std::mutex> lock(this->mutex);
    this->off_item_texture = t;
}

void Player::set_planks_count(uint32_t c) {
    std::scoped_lock<std::mutex> lock(this->mutex);
    this->planks_count = c;
}

void Player::set_coin_count(uint32_t c) {
    std::scoped_lock<std::mutex> lock(this->mutex);
    this->coin_count = c;
}

void Player::move_dxdy(int dx, int dy) {
    std::scoped_lock<std::mutex> lock(this->mutex);
    this->x += dx;
    this->y += dy;
}
