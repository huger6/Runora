#include <texture.hpp>


// Public


TextureManager::TextureManager() = default;

void TextureManager::loadTileset(const std::string& path) {
    if (!tileset.loadFromFile(path))
        std::cout << "Failed to load tileset";
}

const sf::Texture& TextureManager::getTileset() const { return tileset; }

const sf::Texture* TextureManager::getTilesetPtr() const { return &tileset; }