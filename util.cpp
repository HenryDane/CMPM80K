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
