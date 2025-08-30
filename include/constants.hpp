#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

inline sf::Font& getFont() {
    static sf::Font font;
    static bool loaded = false;

    if (!loaded) {
        if (!font.openFromFile("assets/fonts/CookieCrisp-L36ly/CookieCrisp-L36ly.ttf")) 
            throw std::runtime_error("Failed to load font");
        loaded = true;
    }

    return font;
}

inline sf::Texture& getBackgroundTexture() {
    static sf::Texture bgTexture;
    static bool loaded = false;

    if (!loaded) {
        if (!bgTexture.loadFromFile("assets/textures/bg-menu.png")) 
            throw std::runtime_error("Failed to load background");
        loaded = true;
    }

    return bgTexture;
}

inline sf::Texture& getBackgroundTexturePause() {
    static sf::Texture bgTexture;
    static bool loaded = false;

    if (!loaded) {
        if (!bgTexture.loadFromFile("assets/textures/fundo.jpg")) 
            throw std::runtime_error("Failed to load background");
        loaded = true;
    }

    return bgTexture;
}