#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class TextureManager {
    private:
        sf::Texture tileset;
    public:
        TextureManager() = default;

        void loadTileset(const std::string& path) {
            if (!tileset.loadFromFile(path))
                std::cout << "Failed to load tileset";
        }

        const sf::Texture& getTileset() const {
            return tileset;
        }

        const sf::Texture* getTilesetPtr() const {
            return &tileset;
        }
};