#pragma once

#include <iostream>
#include <string>
// SFML
#include <SFML/Graphics.hpp>

class TextureManager {
    public:
        TextureManager();

        void loadTileset(const std::string& path);

        const sf::Texture& getTileset() const;

        const sf::Texture* getTilesetPtr() const;
    private:
        sf::Texture tileset;
};