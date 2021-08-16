#include <iostream>
#include <SFML/Graphics.hpp>
#include "main.h"
#include "map.h"
#include "draw.h"
#include "config.h"
#include "game.h"

// global render texture for reasons idk im tired
sf::RenderTexture renderTexture;

// entity information
player_t player;
uint32_t g_next_uuid = 0xA;

// global state information
int game_state = 0;
int turns_remaining = 100;

// map and play information
Map* current_map;
Config* config;

// TODO menu information

// function prototypes
void process_key_menu(sf::Keyboard::Key k);
void process_key_play(sf::Keyboard::Key k);
void process_key_special(sf::Keyboard::Key k);
void process_key_pause(sf::Keyboard::Key k);

int main() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_W, SCREEN_H), "CMPM 80K Graphical");
    if (!renderTexture.create(SCREEN_W, SCREEN_H)) {
        std::cout << "could not create render texture!" << std::endl;
        return 1020;
    }

    init_draw();

    // initalize everything
    config = new Config();
    player = {config->get_player_sx(), config->get_player_sy(), 5, 0, 0, 0};
    // TODO make this load from startup file
    //current_map = new Map(config->get_map_id(), "__none__", config->get_map_w(), config->get_map_h());
    current_map = new Map("asset/Map02_Groud.csv", "asset/Map02_Animoo.csv", config->get_map_w(), config->get_map_h());

    game_state = 10;

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
                case 10:
                    process_key_play(event.key.code);
                    break;
                case 11:
                case 12:
                    process_key_special(event.key.code);
                    break;
                case 50:
                    process_key_pause(event.key.code);
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
        case 50:
            draw_pause_menu();
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
        window.draw(sprite);

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
    case sf::Keyboard::E:
        // TODO interact???
        break;
    case sf::Keyboard::Escape:
        // TODO goto pause screen
        break;
    default:
        break;
    }
}

void process_key_menu(sf::Keyboard::Key k) {

}

void process_key_special(sf::Keyboard::Key k) {

}

void process_key_pause(sf::Keyboard::Key k) {

}
