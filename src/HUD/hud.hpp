#pragma once

#include <iostream>
#include <string>
// SFML
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
// Timer
#include <time.hpp>
// Orbs
#include <orbCounter.hpp>


class HUD {
    public:
        HUD();

        sf::Time getElapsed() const;
        uint32_t getTotalOrbs() const;

        void update(uint32_t orbsToAdd);
        void draw(sf::RenderWindow& window, sf::View& camera);
    private:
        GameTimer timer;
        OrbCounter orbs;
};