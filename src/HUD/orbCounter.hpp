#pragma once

#include <iostream>
#include <string>
// SFML
#include <SFML/Graphics.hpp>


class OrbCounter {
    private:
        uint32_t orbsTotal;
        sf::Text text;
        
        void setCounterPosition(sf::View& view);
    public:
        explicit OrbCounter();

        uint32_t getTotalOrbs() const;

        void update(uint32_t orbs);
        void draw(sf::RenderWindow& window, sf::View& view);
};