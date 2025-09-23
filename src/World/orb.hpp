#pragma once

#include <iostream>
#include <string>
// SFML
#include <SFML/Graphics.hpp>


namespace OrbConfigs {
    constexpr float ORB_RADIUS = 22.0f;
    constexpr float ORB_SPAWN_PROBABILITY = .15f;
}


struct OrbInfo {
    uint16_t id;
    std::string name;
    uint16_t x, y;
    sf::IntRect textureRect;
};


class Orb {
    public:
        Orb(sf::Vector2i position);

        void drawHitbox(sf::RenderWindow& window);
        bool intersectsHitbox(const sf::FloatRect& playerHitbox);

        void setCollected();
    private:
        bool collected;
        sf::CircleShape hitbox;
        sf::Vector2i position;
    
        void setHibbox();
};


class OrbRegistry {
    public:
        OrbRegistry();

        void loadFromJSON(std::string filename);

        const OrbInfo* getOrbInfo(const uint16_t id) const;
        std::optional<sf::IntRect> getTextureRect(const uint16_t id) const;
    private:
        inline static std::unordered_map<uint16_t, OrbInfo> orbs;
};
