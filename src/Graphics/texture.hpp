#pragma once

#include <iostream>
#include <string>
// SFML
#include <SFML/Graphics.hpp>

class TextureManager {
    public:
        TextureManager();

        void loadSet(const std::string& path);

        const sf::Texture& getSet() const;

        const sf::Texture* getSetPtr() const;
    private:
        sf::Texture set;
};
