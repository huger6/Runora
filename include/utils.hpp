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

inline bool isEscClicked() {
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape);
}

inline void setMouseAsHand(sf::RenderWindow& window) {
    const auto cursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Hand).value();
    window.setMouseCursor(cursor);
}

inline void setMouseAsArrow(sf::RenderWindow& window) {
    const auto cursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Arrow).value();
    window.setMouseCursor(cursor);
}