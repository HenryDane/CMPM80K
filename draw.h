#ifndef _DRAW_H
#define _DRAW_H

#include <SFML/Graphics.hpp>
#include <string>

bool init_draw(void);

bool jumpwr_xyt(int x, int y, int t);
bool jumptex_xyt(int x, int y, std::string text);
bool jumptex_xyt(int x, int y, std::string text, sf::Color c);
bool clear_display(void);

void draw_loading_screen();
void draw_pause_menu();
void draw_defeat_screen();
void draw_victory_screen();
void draw_main_menu();
void draw_player();
void draw_entities();
void draw_current_map();
void draw_hud();
void draw_credits();
void draw_opening_curscene();
void draw_dialogue();
void draw_save_notif();
void draw_quit_confirm();

extern sf::Font font;

#endif // _DRAW_H
