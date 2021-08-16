#include <iostream>
#include <SFML/Graphics.hpp>
#include "main.h"
#include "map.h"
#include "draw.h"
#include "config.h"
#include "game.h"

// global render texture for reasons idk im tired
sf::RenderTexture renderTexture;
sf::RenderWindow* renderWindow;

// entity information
player_t player;
uint32_t g_next_uuid = 0xA;

// global state information
/*
    GAME STATE
    0 -> uninitalized
    2 -> main menu
    3 -> credits
    10 -> normal
    11 -> victory
    12 -> defeat
    50 -> pause
    51 -> confirm quit
    52 -> save notif
*/
int game_state = 0;
int turns_remaining = 9999;

// map and play information
Map* current_map;
Config* config;

// TODO menu information
int current_menu_sel = 0;

// function prototypes
void process_key_menu(sf::Keyboard::Key k);
void process_key_play(sf::Keyboard::Key k);
void process_key_special(sf::Keyboard::Key k);
void process_key_pause(sf::Keyboard::Key k);
void process_key_credits(sf::Keyboard::Key k);

int main() {
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(SCREEN_W, SCREEN_H), "CMPM 80K Graphical");
    renderWindow = &window;
    if (!renderTexture.create(SCREEN_W, SCREEN_H)) {
        std::cout << "could not create render texture!" << std::endl;
        return 1020;
    }

    init_draw();

    // initalize everything
    config = new Config();
    player = {config->get_player_sx(), config->get_player_sy(), 5, T_EMPTY, T_EMPTY, 123, 3};
    //player = {20, 20, 5, T_EMPTY, T_EMPTY, 123, 3};
    // TODO make this load from startup file
    //current_map = new Map(config->get_map_id(), "__none__", config->get_map_w(), config->get_map_h());
    current_map = new Map("asset/test_map_Ground.csv", "asset/test_map_Entities.csv", config->get_map_w(), config->get_map_h());

    game_state = 2;

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
                    std::cout << "X=" << player.x << " Y=" << player.y;
                    std::cout << " TILE=" << (int) current_map->get_tile_at(player.x, player.y) << std::endl;
                }

                switch(game_state) {
                case 1:
                    // do nothing
                    break;
                case 2:
                    process_key_menu(event.key.code);
                    break;
                case 3:
                    process_key_credits(event.key.code);
                    break;
                case 10:
                    process_key_play(event.key.code);
                    break;
                case 11:
                case 12:
                    process_key_special(event.key.code);
                    break;
                case 50:
                    process_key_pause(event.key.code);
                    break;
                case 51:
                    // TODO process confirm quit
                    break;
                case 52:
                    // TODO process save notif
                    break;
                default:
                    if (event.key.code == sf::Keyboard::Tilde) {
                        window.close();
                    }
                    break;
                }
            }
        }

        if (game_state == 10) {
            // TODO check collisions
            // TODO update entities
            // TODO update interactions
        }

        window.clear();
        switch(game_state) {
        case 1:
            draw_loading_screen();
            break;
        case 2:
            draw_main_menu();
            break;
        case 3:
            draw_credits();
            break;
        case 50:
            //draw_pause_menu();
        case 10:
            draw_current_map();
            draw_entities();
            draw_player();
            break;
        case 11:
            draw_victory_screen();
            break;
        case 12:
            draw_defeat_screen();
            break;
        }

        // prep the render texture
        renderTexture.display();

        // get the target texture (where the stuff has been drawn)
        const sf::Texture& texture = renderTexture.getTexture();

        // draw it to the window
        sf::Sprite sprite(texture);
        if (game_state == 10 || game_state == 50) {
            sprite.scale(2, 2);
            sprite.setPosition(-16 * 20, -16 * 15);
        }
        window.draw(sprite);

        if (game_state == 10 || game_state == 50) {
            draw_hud();
        }
        if (game_state == 50) {
            draw_pause_menu();
        }

        window.display();
    }

    return 0;
}

void process_key_play(sf::Keyboard::Key k) {
    switch(k) {
        // TODO validate move!
    case sf::Keyboard::W:
        update_player(0, -1);
        break;
    case sf::Keyboard::A:
        update_player(-1, 0);
        break;
    case sf::Keyboard::S:
        update_player(0, 1);
        break;
    case sf::Keyboard::D:
        update_player(1, 0);
        break;
    case sf::Keyboard::Z:
        // TODO use held item
        break;
    case sf::Keyboard::X:
        // TODO use off item
        break;
    case sf::Keyboard::Escape:
        game_state = 50;
        break;
    default:
        break;
    }
}

void process_key_menu(sf::Keyboard::Key k) {
    switch(k) {
    case sf::Keyboard::A:
        if (current_menu_sel == 0) {
            // goto normal gameplay
            // TODO -- make sure this initalizes everything
            std::cout << "update state!" << std::endl;
            game_state = 10;
            return;
        } else if (current_menu_sel == 1) {
            // try to save
            // TODO play error noise
        } else if (current_menu_sel == 2) {
            // TODO show credits
            game_state = 3;
        } else if (current_menu_sel == 3) {
            renderWindow->close();
        }
        break;
    case sf::Keyboard::Z:
        // down
        current_menu_sel++;
        current_menu_sel = current_menu_sel % 4;
        break;
    case sf::Keyboard::X:
        // up
        current_menu_sel--;
        if (current_menu_sel < 0) {
            current_menu_sel = 3;
        }
        break;
    default:
        break;
    }
}

void process_key_special(sf::Keyboard::Key k) {

}

void process_key_pause(sf::Keyboard::Key k) {
    switch(k) {
    case sf::Keyboard::A:
        game_state = 10;
        break;
    case sf::Keyboard::Z:
        // save
        std::cout << "saved" << std::endl;
        break;
    case sf::Keyboard::X:
        // quit to menu
        std::cout << "menu!" << std::endl;
        game_state = 2;
        break;
    case sf::Keyboard::Escape:
        game_state = 10;
        break;
    default:
        break;
    }
}

void process_key_credits(sf::Keyboard::Key k) {
    if (k == sf::Keyboard::X) {
        game_state = 2;
    }
}
