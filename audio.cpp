#include "SFML/Audio.hpp"
#include <map>
#include <iostream>
#include "audio.h"

std::map<SAMPLETYPE, sf::Sound*> sound_table;
std::map<std::string, sf::SoundBuffer*> buffer_list;

void _load_and_register_buffer(std::string filename, std::string save_as, SAMPLETYPE sample) {
    sf::SoundBuffer* buffer = new sf::SoundBuffer();
    if (!buffer->loadFromFile(filename)){
        std::cout << "ERROR: Unable to load audio from path: " << filename << std::endl;
        exit(192);
    }
    buffer_list.insert({save_as, buffer});

    if (sample != SAMPLETYPE::NONE) {
        sf::Sound* sound = new sf::Sound(*buffer);
        sound_table.insert({sample, sound});
    }
}

void _register_sound(std::string sound_name, SAMPLETYPE sample) {
    sf::SoundBuffer* buffer = buffer_list[sound_name];
    sf::Sound* sound = new sf::Sound(*buffer);
    sound_table.insert({sample, sound});
}

void init_audio() {
    _load_and_register_buffer("asset/audio/click.wav", "click", SAMPLETYPE::UI_CLICK);
    _load_and_register_buffer("asset/audio/select.wav", "select", SAMPLETYPE::UI_SELECT);

    _load_and_register_buffer("asset/audio/win.wav", "win", SAMPLETYPE::WIN);
    _load_and_register_buffer("asset/audio/lose.flac", "lose", SAMPLETYPE::LOSE);

    _load_and_register_buffer("asset/audio/sword.wav", "attack", SAMPLETYPE::ATTACK);
    _load_and_register_buffer("asset/audio/money.wav", "money", SAMPLETYPE::COIN_COLLECT);
    _load_and_register_buffer("asset/audio/heart.wav", "heart", SAMPLETYPE::HEART_COLLECT);
    _load_and_register_buffer("asset/audio/planks.wav", "planks", SAMPLETYPE::WOOD_COLLECT);
    _register_sound("planks", SAMPLETYPE::WOOD_CUT);
    _load_and_register_buffer("asset/audio/chest.wav", "chest", SAMPLETYPE::CHEST_OPEN);
    _load_and_register_buffer("asset/audio/item.wav", "item", SAMPLETYPE::ITEM_PICKUP);

    _load_and_register_buffer("asset/audio/sheep_passive.flac", "sheep_passive", SAMPLETYPE::SHEEP_PASSIVE);
    _load_and_register_buffer("asset/audio/sheep_collect.flac", "sheep_collect", SAMPLETYPE::SHEEP_COLLECT);
    _load_and_register_buffer("asset/audio/cow.wav", "cow", SAMPLETYPE::COW_COLLECT);
    _register_sound("cow", SAMPLETYPE::COW_PASSIVE);
    _load_and_register_buffer("asset/audio/pig_passive.wav", "pig_passive", SAMPLETYPE::PIG_PASSIVE);
    _load_and_register_buffer("asset/audio/pig_collect.wav", "pig_collect", SAMPLETYPE::PIG_COLLECT);
    _load_and_register_buffer("asset/audio/chicken_passive.wav", "chicken_passive", SAMPLETYPE::CHICKEN_PASSIVE);
    _load_and_register_buffer("asset/audio/chicken_collect.wav", "chicken_collect", SAMPLETYPE::CHICKEN_COLLECT);
}

void stop_audio() {
    for (auto& [key, value] : sound_table) {
        delete value;
    }

    for (auto& [key, value] : buffer_list) {
        delete value;
    }
}

void trigger_sound(SAMPLETYPE sample) {
    if (sample == SAMPLETYPE::NONE) {
        return;
    }

    if (sound_table.find(sample) == sound_table.end()) {
        return;
    }

    std::cout << "TRIGGERING SOUND: " << (int) sample << std::endl;
    sound_table[sample]->play();
}
