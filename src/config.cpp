#include "../include/config.h"


ConfigParser::ConfigParser(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("file " + filename + " not found");
    }
    parse(file);
}


void ConfigParser::parse(std::ifstream& file) {
    std::string line;
    while( std::getline(file, line) ) {
        std::istringstream is_line(line);
        std::string key;
        if( std::getline(is_line, key, '=') ) {
            std::string value;
            if ( std::getline(is_line, value) ) {
                options[key] = value;
            }
        }
    }
}
