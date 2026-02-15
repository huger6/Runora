#pragma once

#include <iostream>
// SFML
#include <SFML/Graphics.hpp>


struct Vector2iHash {
    std::size_t operator()(const sf::Vector2i& p) const noexcept {
        std::size_t h1 = std::hash<int>{}(p.x);
        std::size_t h2 = std::hash<int>{}(p.y);
        return h1 ^ (h2 << 1);
    }
};

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
            std::cout << "Failed to load background texture";
        loaded = true;
    }

    return bgTexture;
}

inline const std::string& getBackgroundTexturePath() {
    static const std::string path = "assets/textures/bg-menu.png";
    return path;
}

inline sf::Texture& getBackgroundTexturePause() {
    static sf::Texture bgTexture;
    static bool loaded = false;

    if (!loaded) {
        if (!bgTexture.loadFromFile("assets/textures/fundo.jpg")) 
            std::cout << "Failed to load background texture";
        loaded = true;
    }

    return bgTexture;
}

inline const std::string& getBackgroundTexturePausePath() {
    static const std::string path = "assets/textures/fundo.jpg";
    return path;
}

inline sf::Texture& getPlayerTexture() {
    static sf::Texture playerTexture;
    static bool loaded = false;

    if (!loaded) {
        if (!playerTexture.loadFromFile("assets/textures/character-runora.png")) {
            std::cout << "Failed to load player texture";
        }
        loaded = true;
    }

    return playerTexture;
}

inline sf::Texture& getAuraOrbTexture() {
    static sf::Texture orb;
    static bool loaded = false;

    if (!loaded) {
        if (!orb.loadFromFile("assets/textures/aura-orb-runora.png")) {
            std::cout << "Failed to load Aura Orb texture";
        }
        loaded = true;
    }

    return orb;
}

inline const std::string& getTilesetPath() {
    static const std::string path = "assets/tileset/tileset-runora.png";
    return path;
}

inline const std::string& getTilesetJSONPath() {
    static const std::string path = "assets/tileset/tileset.json";
    return path;
}

inline const std::string& getOrbsetJSONPath() {
    static const std::string path = "assets/orbs/orbset.json";
    return path;
}

inline const std::string& getOrbPath() {
    static const std::string path = "assets/orbs/orbset.png";
    return path;
}
