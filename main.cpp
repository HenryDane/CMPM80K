#include <iostream>
#include <thread>
#include <mutex>
#include <SFML/Graphics.hpp>
#include <chrono>
#include "main.h"
#include "map.h"
#include "draw.h"
#include "config.h"
#include "game.h"
#include "gamemanager.h"

// global variables
sf::RenderTexture renderTexture;
sf::RenderWindow* renderWindow;

uint32_t g_next_uuid = 0xA;
int current_menu_sel = 0;

Player* player; // TODO make this thread safe
Map* current_map;
Config* config;
ark_t ark; // TODO make this a class
GameManager* game;

// function prototypes
void poll_input_direct();
bool process_key_menu();
bool process_key_play();
bool process_key_special();
bool process_key_pause();
bool process_key_credits();
sf::Clock input_timer;

int main() {
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(SCREEN_W, SCREEN_H), "CMPM 80K Graphical", sf::Style::Close);
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(60);
    renderWindow = &window;
    if (!renderTexture.create(SCREEN_W, SCREEN_H)) {
        std::cout << "could not create render texture!" << std::endl;
        return 1020;
    }

    // initalize everything
    init_draw();
    input_timer.restart();
    game = new GameManager();
    config = new Config(); // TODO make this useful
    player = new Player(20, 20, 5); // TODO fix this (sign compare, hardcoded)
    ark = {0, 0, 0, 0, -1, -1, false};
    current_map = new Map(config->get_map_id_ground(), config->get_map_id_entity(), config->get_map_w(), config->get_map_h());
    game->alter_game_state(GameManager::MAIN_MENU);

    while (window.isOpen()) {
        window.clear();
        renderTexture.clear();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::Resized) {
                float resY = event.size.height;
                float resX = event.size.width;

                int newH = (WINDOW_WIDTH*resY)/resX;
                int displace = (newH - WINDOW_HEIGHT)/(-2);

                window.setView(sf::View(sf::FloatRect(0, displace, WINDOW_WIDTH, newH)));
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Tab) {
                    std::cout << "X=" << player->get_x() << " Y=" << player->get_y();
                    std::cout << " TILE=" << (int) current_map->get_tile_at(player->get_x(), player->get_y());
                    std::cout << " STATE=" << game->get_game_state() << std::endl;
                }
            }
        }

        poll_input_direct();

        window.clear();
        switch(game->get_game_state()) {
        case GameManager::STARTING:
            draw_loading_screen();
            break;
        case GameManager::MAIN_MENU:
            draw_main_menu();
            break;
        case GameManager::CREDITS:
            draw_credits();
            break;
        case GameManager::PAUSED:
            //draw_pause_menu();
        case GameManager::NORMAL_PLAY:
            draw_current_map();
            draw_entities();
            draw_player();
            break;
        case GameManager::WIN_CUTSCENE:
            draw_victory_screen();
            break;
        case GameManager::LOOSE_CUTSCENE:
            draw_defeat_screen();
            break;
        case GameManager::OPEN_CUTSCENE:
            draw_opening_curscene();
            break;
        default:
            break;
        }

        // prep the render texture
        renderTexture.display();

        // get the target texture (where the stuff has been drawn)
        const sf::Texture& texture = renderTexture.getTexture();

        // draw it to the window
        sf::Sprite sprite(texture);
        if (game->get_game_state() == GameManager::NORMAL_PLAY ||
            game->get_game_state() == GameManager::PAUSED) {
            sprite.scale(2, 2);
            sprite.setPosition(-16 * 20, -16 * 15);
        }
        window.draw(sprite);

        if (game->get_game_state() == GameManager::NORMAL_PLAY ||
            game->get_game_state() == GameManager::PAUSED) {
            draw_hud();
        }
        if (game->get_game_state() == GameManager::PAUSED) {
            draw_pause_menu();
        }

        window.display();
    }

    game->await_shutdown();

    return 0;
}


void poll_input_direct() {
    if (!renderWindow->hasFocus()) {
        return;
    }

    bool state = true;

    sf::Time time = input_timer.getElapsedTime();
    if (time.asMilliseconds() < 130) {
        return;
    }

    switch(game->get_game_state()) {
//    case 1:
//        // do nothing
//        break;
    case GameManager::MAIN_MENU:
        state = process_key_menu();
        break;
    case GameManager::CREDITS:
        state = process_key_credits();
        break;
    case GameManager::NORMAL_PLAY:
        state = process_key_play();
        break;
    case GameManager::OPEN_CUTSCENE:
    case GameManager::WIN_CUTSCENE:
    case GameManager::LOOSE_CUTSCENE:
        state = process_key_special();
        break;
    case GameManager::PAUSED:
        state = process_key_pause();
        break;
    case GameManager::CONFIRM_QUIT:
        // TODO process confirm quit
        break;
    case GameManager::CONFIRM_SAVE:
        // TODO process save notif
        break;
    default:
        break;
    }

    if (state) {
        // no keys were pressed
        input_timer.restart();
    }
}

bool process_key_play() {
    bool pressed = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        update_player(0, -1);
        pressed = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        update_player(-1, 0);
        pressed = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        update_player(0, 1);
        pressed = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        update_player(1, 0);
        pressed = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
        if (player->get_held_item_texture() != T_EMPTY) {
            do_drop_item();
        } else {
            do_pickup_item();
        }
        pressed = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        // TODO use off item
        pressed = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
//        game_state = 50;
//        edit_timer_state(false);
        game->alter_game_state(GameManager::PAUSED);
        pressed = true;
    }
    return pressed;
}

bool process_key_menu() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        if (current_menu_sel == 0) {
            // goto normal gameplay
            // TODO -- make sure this initalizes everything
            std::cout << "update state!" << std::endl;
//            edit_timer_state(false);
//            game_state = 13;
            game->alter_game_state(GameManager::OPEN_CUTSCENE);
            return false;
        } else if (current_menu_sel == 1) {
            // try to save
            // TODO play error noise
        } else if (current_menu_sel == 2) {
            // TODO show credits
//            edit_timer_state(false);
//            game_state = 3;
            game->alter_game_state(GameManager::CREDITS);
        } else if (current_menu_sel == 3) {
            renderWindow->close();
        }
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
        // down
        current_menu_sel++;
        current_menu_sel = current_menu_sel % 4;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        // up
        current_menu_sel--;
        if (current_menu_sel < 0) {
            current_menu_sel = 3;
        }
    } else {
        return false;
    }
    return true;
}

bool process_key_special() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        if (game->get_game_state() == GameManager::OPEN_CUTSCENE) {
            // since were in opening, we go to normal
//            edit_timer_state(true);
//            game_state = 10;
            game->alter_game_state(GameManager::NORMAL_PLAY);
            // lock timer mutex and enable it
        } else {
//            edit_timer_state(false);
//            game_state = 2;
            game->alter_game_state(GameManager::MAIN_MENU);
        }
    } else {
        return false;
    }
    return true;
}

bool process_key_pause() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
//        edit_timer_state(true);
//        game_state = 10;
        game->alter_game_state(GameManager::NORMAL_PLAY);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
        // TODO save
        std::cout << "saved" << std::endl;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        // quit to menu
//        edit_timer_state(false);
//        game_state = 2;
        game->alter_game_state(GameManager::MAIN_MENU);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
//        edit_timer_state(true);
//        game_state = 10;
        game->alter_game_state(GameManager::NORMAL_PLAY);
    } else {
        return false;
    }
    return true;
}

bool process_key_credits() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        // quit to menu
//        edit_timer_state(false);
//        game_state = 2;
        game->alter_game_state(GameManager::MAIN_MENU);
    } else {
        return false;
    }
    return true;
}
