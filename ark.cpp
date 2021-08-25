#include "ark.h"

Ark::Ark(int x, int y) {
    this->x = x;
    this->y = y;

    this->on_map = false;
    this->exists = false;

    this->planks_count = 0;
    this->chickens = 0;
    this->cows = 0;
    this->pigs = 0;
    this->sheep = 0;
}

Ark::~Ark() {

}
