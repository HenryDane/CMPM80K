#include "util.h"
#include <sstream>

std::vector<std::string> split_by_char(std::string in, char split) {
    std::stringstream stream(in);
    std::vector<std::string> tokens;
    std::string token;
    while(std::getline(stream, token, split)) {
        tokens.push_back(token);
    }
    return tokens;
}

bool is_texture_animal(int t) {
    if (t >= 41 && t <= 43) {
        return true;
    }
    if (t == 65) {
        return true;
    }
    return false;
}
bool is_entity_animal(int e_type) {
    if (e_type >= 12 && e_type <= 15) {
        return true;
    }
    return false;
}
bool is_entity_pickupable(int e_type) {
    if (e_type >= 12 && e_type <= 15) {
        return true;
    }
    if (e_type == E_PLANKS) {
        return true;
    }
    return false;
}
int get_entity_type_from_texture(int t) {
    if (t >= 41 && t <= 43) {
        return t - 29;
    } else if (t == 65) {
        return 15;
    } else if (t == 62) {
        return 20;
    } else if (t == 61) {
        return 1;
    } else if (t == 63) {
        return 22;
    } else if (t == 60) {
        return 2;
    } else if (t == 64) {
        return 21;
    } else if (t == 53) {
        return 3;
    } else if (t >= 71 && t <= 74) {
        return t - 31;
    }
    return 0;
}
