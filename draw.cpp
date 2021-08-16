#include <vector>
#include <iostream>
#include "draw.h"
#include "main.h"

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

void draw_hud() {
    const float xoff = 128.0f;
    char tmp[16];

    sf::RectangleShape background;
    background.setPosition(0, 0);
    background.setSize(sf::Vector2f(640, 6*16));
    background.setFillColor(sf::Color(0, 0, 0, 255));
    renderWindow->draw(background);

    sf::RectangleShape held_item_frame;
    held_item_frame.setPosition(32 + xoff, 32);
    held_item_frame.setSize(sf::Vector2f(2, 3));
    held_item_frame.setScale(16, 16);
    held_item_frame.setFillColor(sf::Color(0, 0, 0, 255));
    held_item_frame.setOutlineColor(sf::Color(255, 0, 0, 255));
    held_item_frame.setOutlineThickness(3 / 16.0f);
    renderWindow->draw(held_item_frame);

    sf::Sprite held_item;
    held_item.setTexture(textures.at(player.held_item_texture));
    held_item.setPosition(32 + 4 + xoff, 32 + 12);
    held_item.setScale(1.5f, 1.5f); // (128 / 4) = 32
    renderWindow->draw(held_item);

    sf::RectangleShape off_item_frame;
    off_item_frame.setPosition(80 + xoff, 32);
    off_item_frame.setSize(sf::Vector2f(2, 3));
    off_item_frame.setScale(16, 16);
    off_item_frame.setFillColor(sf::Color(0, 0, 0, 255));
    off_item_frame.setOutlineColor(sf::Color(0, 0, 255, 255));
    off_item_frame.setOutlineThickness(3 / 16.0f);
    renderWindow->draw(off_item_frame);

    sf::Sprite off_item;
    off_item.setTexture(textures.at(player.off_item_texture));
    off_item.setPosition(80 + 4 + xoff, 32 + 12);
    off_item.setScale(1.5f, 1.5f); // (128 / 4) = 32
    renderWindow->draw(off_item);

    sf::Text coin_count;
    sprintf(tmp, " COINS x%.3d", player.coin_count);
    coin_count.setString(tmp);
    coin_count.setCharacterSize(14);
    coin_count.setPosition(128 + xoff, 32);
    coin_count.setFillColor(sf::Color(255, 255, 255, 255));
    coin_count.setFont(font);
    renderWindow->draw(coin_count);

    sf::Text plank_count;
    sprintf(tmp, "PLANKS x%.3d", player.planks_count);
    plank_count.setString(tmp);
    plank_count.setCharacterSize(14);
    plank_count.setPosition(128 + xoff, 64);
    plank_count.setFillColor(sf::Color(255, 255, 255, 255));
    plank_count.setFont(font);
    renderWindow->draw(plank_count);

    sf::Text timer_text;
    if (turns_remaining > 999) {
        timer_text.setString("TIMER >999");
    } else {
        sprintf(tmp, "TIMER x%.3d", turns_remaining);
        timer_text.setString(tmp);
    }
    timer_text.setCharacterSize(14);
    timer_text.setPosition(256 + xoff, 64);
    timer_text.setFillColor(sf::Color(255, 255, 255, 255));
    timer_text.setFont(font);
    renderWindow->draw(timer_text);

    for (int i = 0; i < player.hearts; i++) {
        sf::Sprite heart;
        heart.setTexture(textures.at(T_HEART));
        heart.setPosition(256 + i * 16 * 1.5 + xoff, 32);
        heart.setScale(1.5f, 1.5f); // (128 / 4) = 32
        renderWindow->draw(heart);
    }

    sf::Sprite key_label;
    key_label.setTexture(textures.at(T_KEY_Z));
    key_label.setPosition(40 + xoff, 8);
    key_label.setScale(1.0f, 1.0f); // (128 / 4) = 32
    if (game_state != 10) key_label.setColor(sf::Color(128, 128, 128, 255));
    renderWindow->draw(key_label);

    //sf::Sprite key_label;
    key_label.setTexture(textures.at(T_KEY_X));
    key_label.setPosition(88 + xoff, 8);
    key_label.setScale(1.0f, 1.0f); // (128 / 4) = 32
    renderWindow->draw(key_label);
}

/*
===================================================================================================
  DRAWING THE MENU
===================================================================================================
*/

void draw_main_menu() {
    sf::Text title;
    title.setString("NOAH'S ARK");
    title.setCharacterSize(28);
    title.setFillColor(sf::Color(255, 255, 255));
    title.setFont(font);
    sf::FloatRect fr = title.getLocalBounds();
    title.setPosition(10, (480 - fr.height) / 2);
    renderTexture.draw(title);

    sf::Sprite key_label;
    key_label.setTexture(textures.at(T_KEY_X));
    key_label.setPosition(10, 480 - 32);
    key_label.setScale(1.0f, 1.0f); // (128 / 4) = 32
    //key_label.setColor(sf::Color(128, 128, 128, 255));
    renderTexture.draw(key_label);

    //sf::Sprite key_label;
    key_label.setTexture(textures.at(T_KEY_Z));
    key_label.setPosition(10 + 80, 480 - 32);
    key_label.setScale(1.0f, 1.0f); // (128 / 4) = 32
    renderTexture.draw(key_label);

    key_label.setTexture(textures.at(T_KEY_A));
    key_label.setPosition(10 + 180, 480 - 32);
    key_label.setScale(1.0f, 1.0f); // (128 / 4) = 32
    renderTexture.draw(key_label);

    sf::Text text;
    text.setString("UP");
    text.setCharacterSize(14);
    text.setFillColor(sf::Color(255, 255, 255));
    text.setFont(font);
    text.setPosition(10 + 32, 480 - 32);
    renderTexture.draw(text);
    text.setString("DOWN");
    text.setPosition(10 + 112, 480 - 32);
    renderTexture.draw(text);
    text.setString("SELECT");
    text.setPosition(10 + 212, 480 - 32);
    renderTexture.draw(text);

    if (current_menu_sel == 0) {
        text.setString("> NEW GAME");
        text.setFillColor(sf::Color(0, 255, 0));
    } else {
        text.setString("NEW GAME");
        text.setFillColor(sf::Color(0, 128, 0));
    }
    fr = text.getLocalBounds();
    float height_ref = (480 - fr.height) / 2;
    text.setPosition(640 - fr.width - 10, height_ref - 48);
    renderTexture.draw(text);

    if (current_menu_sel == 1) {
        text.setString("> LOAD GAME");
        text.setFillColor(sf::Color(0, 255, 0));
    } else {
        text.setString("LOAD GAME");
        text.setFillColor(sf::Color(0, 128, 0));
    }
    fr = text.getLocalBounds();
    text.setPosition(640 - fr.width - 10, height_ref - 16);
    renderTexture.draw(text);

    if (current_menu_sel == 2) {
        text.setString("> CREDITS");
        text.setFillColor(sf::Color(0, 255, 0));
    } else {
        text.setString("CREDITS");
        text.setFillColor(sf::Color(0, 128, 0));
    }
    fr = text.getLocalBounds();
    text.setPosition(640 - fr.width - 10, height_ref + 16);
    renderTexture.draw(text);

    if (current_menu_sel == 3) {
        text.setString("> QUIT");
        text.setFillColor(sf::Color(0, 255, 0));
    } else {
        text.setString("QUIT");
        text.setFillColor(sf::Color(0, 128, 0));
    }
    fr = text.getLocalBounds();
    text.setPosition(640 - fr.width - 10, height_ref + 48);
    renderTexture.draw(text);
}

void draw_credits() {
    sf::Text title;
    title.setString("NOAH'S ARK");
    title.setCharacterSize(28);
    title.setFillColor(sf::Color(255, 255, 255));
    title.setFont(font);
    sf::FloatRect fr = title.getLocalBounds();
    title.setPosition(10, (480 - fr.height) / 2);
    renderTexture.draw(title);

    sf::Sprite key_label;
    key_label.setTexture(textures.at(T_KEY_X));
    key_label.setPosition(10, 480 - 32);
    key_label.setScale(1.0f, 1.0f); // (128 / 4) = 32
    //key_label.setColor(sf::Color(128, 128, 128, 255));
    renderTexture.draw(key_label);

    sf::Text text;
    text.setString("BACK");
    text.setCharacterSize(14);
    text.setFillColor(sf::Color(255, 255, 255));
    text.setFont(font);
    text.setPosition(10 + 32, 480 - 32);
    renderTexture.draw(text);

    /*
    =============================================
    |       ACTUALLY DISPLAY THE CREDITS        |
    =============================================
    */
    text.setString("PROGRAMMING, ENGINE, CONCEPT");
    fr = text.getLocalBounds();
    text.setPosition(640 - fr.width, 0);
    text.setFillColor(sf::Color(128, 128, 128, 255));
    renderTexture.draw(text);

    text.setString("HENRY OLLING");
    fr = text.getLocalBounds();
    text.setPosition(640 - fr.width, 16);
    text.setFillColor(sf::Color(255, 255, 255));
    renderTexture.draw(text);

    text.setString("ASSETS, CONCEPT");
    fr = text.getLocalBounds();
    text.setPosition(640 - fr.width, 48);
    text.setFillColor(sf::Color(128, 128, 128, 255));
    renderTexture.draw(text);

    text.setString("JOCQUE JEFFERSON");
    fr = text.getLocalBounds();
    text.setPosition(640 - fr.width, 64);
    text.setFillColor(sf::Color(255, 255, 255));
    renderTexture.draw(text);

    text.setString("ASSETS");
    fr = text.getLocalBounds();
    text.setPosition(640 - fr.width, 96);
    text.setFillColor(sf::Color(128, 128, 128, 255));
    renderTexture.draw(text);

    text.setString("@capiwak");
    fr = text.getLocalBounds();
    text.setPosition(640 - fr.width, 112);
    text.setFillColor(sf::Color(255, 255, 255));
    renderTexture.draw(text);

    // TODO ask jocque for other artist credits
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
    sf::RectangleShape background;
    background.setPosition(213, 160);
    background.setSize(sf::Vector2f(213, 160));
    background.setFillColor(sf::Color(0, 0, 0, 255));
    renderWindow->draw(background);

    sf::Text label;
    label.setString("PAUSED");
    label.setCharacterSize(14);
    label.setPosition(213 + 75, 160+20);
    label.setFillColor(sf::Color(255, 255, 255, 255));
    label.setFont(font);
    renderWindow->draw(label);

    label.setString("SAVE");
    label.setCharacterSize(14);
    label.setPosition(213 + 32, 160+52);
    label.setFillColor(sf::Color(255, 255, 255, 255));
    label.setFont(font);
    renderWindow->draw(label);

    label.setString("QUIT TO MENU");
    label.setCharacterSize(14);
    label.setPosition(213 + 32, 160+84);
    label.setFillColor(sf::Color(255, 255, 255, 255));
    label.setFont(font);
    renderWindow->draw(label);

    label.setString("RESUME");
    label.setCharacterSize(14);
    label.setPosition(213 + 32, 160+116);
    label.setFillColor(sf::Color(255, 255, 255, 255));
    label.setFont(font);
    renderWindow->draw(label);

    sf::Sprite key_label;
    key_label.setTexture(textures.at(T_KEY_A));
    key_label.setPosition(213 + 10, 160+116);
    key_label.setScale(1.0f, 1.0f); // (128 / 4) = 32
    renderWindow->draw(key_label);

    //sf::Sprite key_label;
    key_label.setTexture(textures.at(T_KEY_Z));
    key_label.setPosition(213 + 10, 160+52);
    key_label.setScale(1.0f, 1.0f); // (128 / 4) = 32
    renderWindow->draw(key_label);

    //sf::Sprite key_label;
    key_label.setTexture(textures.at(T_KEY_X));
    key_label.setPosition(213 + 10, 160+84);
    key_label.setScale(1.0f, 1.0f); // (128 / 4) = 32
    renderWindow->draw(key_label);
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
