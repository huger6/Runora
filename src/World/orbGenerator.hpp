#pragma once

#include <iostream>
// SFML
#include <SFML/Graphics.hpp>
// World
#include "World/map.hpp"
#include "World/orb.hpp"
// Orb Texture
#include "Graphics/texture.hpp"
// Getters
#include "Utils/getters.hpp"

class OrbGenerator {
    public:
        OrbGenerator(uint16_t arrayLength);

        uint32_t getNumberOfOrbs() const;

        void update(const std::vector<float>& limitsX, const sf::FloatRect& playerHitbox, const sf::View& camera);
        void draw(sf::RenderWindow& window);
    private:
        std::unordered_map<sf::Vector2i, std::unique_ptr<Orb>, Vector2iHash> activeOrbs;
        uint32_t orbCounter;
        sf::VertexArray vertices;
        TextureManager texture;
        OrbRegistry registry;

        bool generateOrb(uint32_t tileX, uint32_t tileY);
        bool spawnOrb(float probability);

        void buildVertexArray();
        void trySpawnOrb(uint32_t tileX, uint32_t tileY);
};