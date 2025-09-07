#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

inline sf::Font& getFont() {
    static sf::Font font;
    static bool loaded = false;

    if (!loaded) {
        if (!font.openFromFile("assets/fonts/CookieCrisp-L36ly/CookieCrisp-L36ly.ttf")) 
            std::cout << "Failed to load text font";
        loaded = true;
    }

    return font;
}

inline sf::Texture& getBackgroundTexture() {
    static sf::Texture bgTexture;
    static bool loaded = false;

    if (!loaded) {
        if (!bgTexture.loadFromFile("assets/textures/bg-menu.png")) 
            std::cout << "Failed to load background";
        loaded = true;
    }

    return bgTexture;
}

inline sf::Texture& getBackgroundTexturePause() {
    static sf::Texture bgTexture;
    static bool loaded = false;

    if (!loaded) {
        if (!bgTexture.loadFromFile("assets/textures/fundo.jpg")) 
            std::cout << ("Failed to load background");
        loaded = true;
    }

    return bgTexture;
}

inline const std::string& getTilesetPath() {
    static const std::string path = "assets/tileset/tileset-runora.png";
    return path;
}

inline const std::string& getTilesetJSONPath() {
    static const std::string path = "assets/tileset/tileset.json";
    return path;
}