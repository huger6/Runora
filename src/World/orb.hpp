#pragma once

#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>
#include <World/map.hpp>
#include <getters.hpp>

namespace OrbConfigs {
    float ORB_RADIUS = 22.0f;
    float ORB_SPAWN_PROBABILITY = .2f;
}

class Orb {
    public:
        Orb();

        void drawHitbox(sf::RenderWindow& window);

        void update(bool collected);
        void draw(sf::RenderWindow& window);
    private:
        bool collected;
        sf::Sprite orbSprite;
        sf::CircleShape hitbox;
    
        void setHibbox();
};


class OrbManager {
    public:
        bool generateOrb(uint32_t tileX, uint32_t tileY);
        bool spawnOrb(float probability);

        void drawHitbox(sf::RenderWindow& window);

        void update(sf::Vector2f playerPos);
        void draw(sf::RenderWindow& window);
    private:
        std::unordered_map<sf::Vector2i, uint32_t, Vector2iHash> activeOrbs;
};