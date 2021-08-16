#include "config.h"

#include <fstream>
#include <string>
#include <sstream>

/*Config::Config() {
    // open config file
    std::ifstream config("asset/startup.txt");
    if (!config.is_open()) {
        std::cout << "ERROR: unable to open startup file!" << std::endl;
        exit(1040);
    }

    // parse each line
    for(std::string line; getline( mapfile, line ); ) {
        // check if map

    }
}*/

Config::Config() {
    map_w = 40;
    map_h = 30;
    start_x = 5;
    start_y = 2;
    map_id = "asset/map01.csv";
}

Config::~Config() {

}
