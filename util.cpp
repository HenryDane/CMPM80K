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
//    if (e_type == E_PLANKS) {
//        return true;
//    }
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

/*
    Shamelessly copied from
    https://github.com/SFML/SFML/wiki/Source%3A-Letterbox-effect-using-a-view
    All credit for this goes to the author(s)
*/
sf::View getLetterboxView(sf::View view, int windowWidth, int windowHeight) {

    // Compares the aspect ratio of the window to the aspect ratio of the view,
    // and sets the view's viewport accordingly in order to archieve a letterbox effect.
    // A new view (with a new viewport set) is returned.

    float windowRatio = windowWidth / (float) windowHeight;
    float viewRatio = view.getSize().x / (float) view.getSize().y;
    float sizeX = 1;
    float sizeY = 1;
    float posX = 0;
    float posY = 0;

    bool horizontalSpacing = true;
    if (windowRatio < viewRatio)
        horizontalSpacing = false;

    // If horizontalSpacing is true, the black bars will appear on the left and right side.
    // Otherwise, the black bars will appear on the top and bottom.

    if (horizontalSpacing) {
        sizeX = viewRatio / windowRatio;
        posX = (1 - sizeX) / 2.f;
    }

    else {
        sizeY = windowRatio / viewRatio;
        posY = (1 - sizeY) / 2.f;
    }

    view.setViewport( sf::FloatRect(posX, posY, sizeX, sizeY) );

    return view;
}
