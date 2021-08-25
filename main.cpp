#include <iostream>
#include <thread>
#include <mutex>
#include <SFML/Graphics.hpp>
#include <chrono>
#include "main.h"
#include "map.h"
#include "draw.h"
#include "game.h"
#include "gamemanager.h"
#include "util.h"

// global variables
sf::RenderTexture renderTexture;
sf::RenderWindow* renderWindow;

uint32_t g_next_uuid = 0xA;
int current_menu_sel = 0;

Player* player;
Map* current_map;
Ark* ark; // TODO make this a class
GameManager* game;
Dialogue* active_dialogue;
int dialogue_state = -1; // <0 -> inactive, >=0 -> active

void reset_game_instance();

// function prototypes
void poll_input_direct();
bool process_key_menu();
bool process_key_play();
bool process_key_special();
bool process_key_pause();
bool process_key_credits();
bool process_key_dialogue();
bool process_key_save_notif();
bool process_key_confirm_quit();
sf::Clock input_timer;

int main() {
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(SCREEN_W, SCREEN_H), "CMPM 80K Graphical");
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(60);
    renderWindow = &window;
    if (!renderTexture.create(SCREEN_W*2, SCREEN_H*2)) {
        std::cout << "could not create render texture!" << std::endl;
        return 1020;
    }

    // initalize everything
    init_draw();
    init_game();
    ark = new Ark(-1, -1);
    input_timer.restart();
    player = new Player(20, 20, 2); // TODO fix this (sign compare, hardcoded)
    game = new GameManager(); // this *should* alter the value of current_map
    active_dialogue = nullptr;
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

//                int newH = (WINDOW_WIDTH*resY)/resX;
//                int displace = (newH - WINDOW_HEIGHT)/(-2);

//                window.setView(sf::View(sf::FloatRect(0, displace, WINDOW_WIDTH, newH)));
                window.setView(getLetterboxView( window.getView(), event.size.width, event.size.height ));
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Tab) {
                    std::cout << "X=" << player->get_x() << " Y=" << player->get_y();
                    std::cout << " TILE=" << (int) current_map->get_tile_at(player->get_x(), player->get_y());
                    std::cout << " STATE=" << game->get_game_state() << std::endl;
                }
            }
        }

        poll_input_direct();
        if (game->get_game_state() == GameManager::NORMAL_PLAY) {
            game->update();
            game_check_win_loose();
        }

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
        case GameManager::CONFIRM_QUIT:
        case GameManager::CONFIRM_SAVE:
        case GameManager::DIALOGUE:
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

        if (game->get_game_state() == GameManager::NORMAL_PLAY ||
            game->get_game_state() == GameManager::PAUSED ||
            game->get_game_state() == GameManager::DIALOGUE ||
            game->get_game_state() == GameManager::CONFIRM_QUIT ||
            game->get_game_state() == GameManager::CONFIRM_SAVE) {
            draw_hud();
        }
        if (game->get_game_state() == GameManager::PAUSED ||
            game->get_game_state() == GameManager::CONFIRM_QUIT ||
            game->get_game_state() == GameManager::CONFIRM_SAVE) {
            draw_pause_menu();
        }
        if (game->get_game_state() == GameManager::DIALOGUE) {
            draw_dialogue();
        } else if (game->get_game_state() == GameManager::CONFIRM_QUIT) {
            draw_quit_confirm();
        } else if (game->get_game_state() == GameManager::CONFIRM_SAVE) {
            draw_save_notif();
        }

        // prep the render texture
        renderTexture.display();

        // get the target texture (where the stuff has been drawn)
        const sf::Texture& texture = renderTexture.getTexture();

        // draw it to the window
        sf::Sprite sprite(texture);
        window.draw(sprite);

        window.display();
    }

    exit_game();
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
        state = process_key_confirm_quit();
        break;
    case GameManager::CONFIRM_SAVE:
        state = process_key_save_notif();
        break;
    case GameManager::DIALOGUE:
        state = process_key_dialogue();
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
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
//        // TODO use off item
//        pressed = true;
//    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
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
            reset_game_instance();
            game->alter_game_state(GameManager::OPEN_CUTSCENE);
            return false;
        } else if (current_menu_sel == 1) {
            // try to save
            // TODO play error noise
        } else if (current_menu_sel == 2) {
            // TODO show credits
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
            game->alter_game_state(GameManager::NORMAL_PLAY);
            // lock timer mutex and enable it
        } else {
            game->alter_game_state(GameManager::MAIN_MENU);
        }
    } else {
        return false;
    }
    return true;
}

bool process_key_pause() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        game->alter_game_state(GameManager::NORMAL_PLAY);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
        // TODO save
        std::cout << "saved" << std::endl;
        game->alter_game_state(GameManager::CONFIRM_SAVE);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        // quit to menu
        game->alter_game_state(GameManager::CONFIRM_QUIT);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        game->alter_game_state(GameManager::NORMAL_PLAY);
    } else {
        return false;
    }
    return true;
}

bool process_key_credits() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        // quit to menu
        game->alter_game_state(GameManager::MAIN_MENU);
    } else {
        return false;
    }
    return true;
}

bool process_key_dialogue() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        dialogue_state++;
        if (dialogue_state >= active_dialogue->text.size()) {
            // done
            active_dialogue = nullptr;
            dialogue_state = -1;
            game->alter_game_state(GameManager::NORMAL_PLAY);
        } else {
            // more to go
        }
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
        active_dialogue = nullptr;
        dialogue_state = -1;
        game->alter_game_state(GameManager::NORMAL_PLAY);
    } else {
        return false;
    }

    return true;
}

bool process_key_save_notif() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        game->alter_game_state(GameManager::PAUSED);
    } else {
        return false;
    }
    return true;
}

bool process_key_confirm_quit() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        game->alter_game_state(GameManager::MAIN_MENU);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
        game->alter_game_state(GameManager::PAUSED);
    } else {
        return false;
    }

    return true;
}

void reset_game_instance() {
    game->destroy_map_data();
    delete player;
    delete ark;
    ark = new Ark(-1, -1);
    player = new Player(20, 20, 2); // TODO fix this (sign compare, hardcoded)
    game->initalize_map_data();

    active_dialogue = nullptr;
    dialogue_state = 0;
}
