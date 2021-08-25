#ifndef _UTIL_H
#define _UTIL_H

#include <string>
#include <vector>
#include "entity.h"
#include <SFML/Graphics.hpp>

std::vector<std::string> split_by_char(std::string in, char split);
bool is_texture_animal(int t);
bool is_entity_animal(int e_type);
bool is_entity_pickupable(int e_type);
int get_entity_type_from_texture(int t);

sf::View getLetterboxView(sf::View view, int windowWidth, int windowHeight);

#endif // _UTIL_H
