#include "texture.hpp"


// Public


TextureManager::TextureManager() = default;

void TextureManager::loadSet(const std::string& path) {
    if (!set.loadFromFile(path))
        std::cout << "Failed to load tileset";
}

const sf::Texture& TextureManager::getSet() const { return set; }

const sf::Texture* TextureManager::getSetPtr() const { return &set; }