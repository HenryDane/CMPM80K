#include <vector>
#include <iostream>
#include "draw.h"
#include "main.h"

#define T_PLAYER 17
#define T_GRASS 8
#define T_BRICK 15

std::vector<sf::Texture> textures;
sf::Font font;

/*
===================================================================================================
  DRAWING THE ACTUAL GAME
===================================================================================================
TODO: switch to tile map eventually tm
*/

void draw_current_map() {
    int dx = 20 - player.x;
    int dy = 15 - player.y;
    for (uint32_t y = 0; y < current_map->get_height(); y++) {
        for (uint32_t x = 0; x < current_map->get_width(); x++) {
            uint8_t t = current_map->get_tile_at(x, y);
            if (t > textures.size()) continue;
            jumpwr_xyt(x + dx, y + dy, t);
        }
    }
}

void draw_entities() {
    int dx = 20 - player.x;
    int dy = 15 - player.y;
    for (Entity& e : current_map->entities) {
        jumpwr_xyt(e.get_x() + dx, e.get_y() + dy, e.get_texture());
    }
}

void draw_player() {
    jumpwr_xyt(20, 15, T_PLAYER);
}

/*
===================================================================================================
  DRAWING THE MENU
===================================================================================================
*/

void draw_main_menu() {

}

/*
===================================================================================================
  DRAWING WIN/LOSS SCREENS
===================================================================================================
*/

void draw_victory_screen() {

}

void draw_defeat_screen() {

}

/*
===================================================================================================
  DRAWING PAUSE SCREEN
===================================================================================================
*/

void draw_pause_menu() {

}

/*
===================================================================================================
  DRAWING LOADING SCREEN
===================================================================================================
*/

void draw_loading_screen() {
    for (unsigned int i = 0; i < textures.size(); i++) {
        jumpwr_xyt(i % 32, i / 32, i);
    }
}

/*
===================================================================================================
  LOWER LEVEL FUNCTIONALITY
===================================================================================================
*/
bool init_draw(void){
    // clear existing tiles
    textures.clear();

    // load our tilemap
    sf::Image tilemap;
    if (!tilemap.loadFromFile("asset/tilemap.png")) {
        std::cout << "ERROR: could not load tilemap!" << std::endl;
        exit(1000);
    }

    // check that its the right size
    sf::Vector2u tilemapsize = tilemap.getSize();
    if (tilemapsize.x % 16 != 0 || tilemapsize.y % 16 != 0) {
        std::cout << "ERROR: tilemap size not a multiple of 16!" << std::endl;
        exit(1001);
    }

    // apply transparency adjust
    for (unsigned int y = 0; y < tilemapsize.y; y++) {
        for (unsigned int x = 0; x < tilemapsize.x; x++) {
            if (tilemap.getPixel(x, y) == sf::Color(255, 255, 255)) {
                tilemap.setPixel(x, y, sf::Color(255, 255, 255, 0));
            }
        }
    }

    // create textures from the result
    for (unsigned int y = 0; y < tilemapsize.y / 16; y++) {
        for (unsigned int x = 0; x < tilemapsize.x / 16; x++) {
            // TODO this might be a performance problem
            sf::Texture texture;
            sf::IntRect region(x * 16, y * 16, 16, 16);
            if (!texture.loadFromImage(tilemap, region)) {
                std::cout << "ERROR: unable to load tile from tilemap: " << x << " " << y << std::endl;
                return 1002;
            }
            textures.push_back(texture);
        }
    }

    // load our font
    if (!font.loadFromFile("asset/telegrama_raw.ttf")) {
        std::cout << "ERROR: could not load font!" << std::endl;
        exit(1010);
    }

    return true;
}
bool jumpwr_xyt(int x, int y, int t) { // tile coords
    // TODO: make the sprites persistent
    sf::Sprite sprite1;
    sprite1.setTexture(textures.at(t));
    sprite1.setPosition(x * TILE_WIDTH, y * TILE_WIDTH);
    sprite1.setScale(TEX_SCALE, TEX_SCALE); // (128 / 4) = 32

    renderTexture.draw(sprite1);
    return true;
}
bool draw_text(int x, int y, std::string text) {
    sf::Text text1;
    text1.setString(text);
    text1.setCharacterSize(TILE_WIDTH - 2);
    text1.setPosition(x, y);
    text1.setFillColor(sf::Color(255, 255, 255));
    text1.setFont(font);

    renderTexture.draw(text1);
    return true;
}
bool jumptex_xyt(int x, int y, std::string text) {
    sf::Text text1;
    text1.setString(text);
    text1.setCharacterSize(TILE_WIDTH - 2);
    text1.setPosition( (x * TILE_WIDTH) + 3, (y * TILE_WIDTH) );
    text1.setFillColor(sf::Color(255, 255, 255));
    text1.setFont(font);

    renderTexture.draw(text1);
    return true;
}
bool jumptex_xyt(int x, int y, std::string text, sf::Color c) {
    sf::Text text1;
    text1.setString(text);
    text1.setCharacterSize(TILE_WIDTH - 2);
    text1.setPosition( (x * TILE_WIDTH) + 3, (y * TILE_WIDTH));
    text1.setFillColor(c);
    text1.setFont(font);

    renderTexture.draw(text1);
    return true;
}
bool clear_display(void) {
    renderTexture.clear();
    return true;
}
