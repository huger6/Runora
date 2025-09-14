#pragma once

#include <iostream>
#include <format>
#include <SFML/Graphics.hpp>


inline void openLink(const std::string& url) {
    #ifdef _WIN32 
        std::string cmd = "start " + url;
    #elif __APPLE__          
        std::string cmd = "open " + url;
    #else
        std::string cmd = "xdg-open " + url;
    #endif 

    std::system(cmd.c_str());
}

inline void setMouseAsHand(sf::RenderWindow& window) {
    const auto cursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Hand).value();
    window.setMouseCursor(cursor);
}

inline void setMouseAsArrow(sf::RenderWindow& window) {
    const auto cursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Arrow).value();
    window.setMouseCursor(cursor);
}


inline std::string capitalize(const std::string& input) {
    if (input.empty()) return input;

    std::string result = input;

    result[0] = std::toupper(static_cast<unsigned char>(result[0]));

    for (size_t i = 1; i < result.length(); i++) {
        result[i] = std::tolower(static_cast<unsigned char>(result[i]));
    }
    return result;
}