#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <World/map.hpp>
#include <getters.hpp>

namespace OrbConfigs {
    float ORB_RADIUS = 22.0f;
}

class Orb {
    private:
        uint32_t orbsCollected;
        std::unordered_map<sf::Vector2i, uint32_t, Vector2iHash> activeOrbs;
        sf::Sprite orbSprite;
        sf::CircleShape hitbox;

        bool generateOrb(uint32_t tileX, uint32_t tileY) {
            
        }

    public:
        Orb() : orbsCollected(0), orbSprite(getAuraOrbTexture()) {
            this->setHibbox(); // Debug
        }

        void setHibbox() {
            hitbox.setRadius(OrbConfigs::ORB_RADIUS);
            hitbox.setOrigin({hitbox.getLocalBounds().size.x / 2.0f, hitbox.getLocalBounds().size.y});
           
            hitbox.setFillColor(sf::Color::Transparent);
            hitbox.setOutlineThickness(1.0f);
            hitbox.setOutlineColor(sf::Color::Red);
        }

        void drawHitbox(sf::RenderWindow& window) {
            for (auto orb = activeOrbs.begin(); orb != activeOrbs.end(); orb++) {
                hitbox.setPosition({static_cast<float>(orb->first.x), static_cast<float>(orb->first.y)});
                window.draw(hitbox);
            }
        }

        void draw(sf::RenderWindow& window) {
            for (auto orb = activeOrbs.begin(); orb != activeOrbs.end(); orb++) {
                orbSprite.setPosition({static_cast<float>(orb->first.x), static_cast<float>(orb->first.y)});
                window.draw(orbSprite);
            }
            
        }
};