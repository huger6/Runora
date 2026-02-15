#pragma once

#include <iostream>
#include <unordered_set>
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
        OrbGenerator(uint16_t arrayLength, OrbRegistry registry);

        uint32_t getNumberOfOrbs() const;

        void update(const sf::Vector2f& playerPos, const sf::FloatRect& playerHitbox, const sf::View& camera);
        void draw(sf::RenderWindow& window);
        void reset();

        void drawHitbox(sf::RenderWindow& window);
    private:
        std::unordered_map<sf::Vector2i, std::unique_ptr<Orb>, Vector2iHash> activeOrbs;
        std::unordered_set<sf::Vector2i, Vector2iHash> processedPositions;
        uint32_t orbCounter;
        sf::VertexArray vertices;
        TextureManager texture;
        OrbRegistry& registry;

        bool generateOrb(uint32_t tileX, uint32_t tile);
        bool spawnOrb(float probability);

        void buildVertexArray();
        void trySpawnOrb(uint32_t tileX, uint32_t tileY);
        void cleanupProcessedPositions(uint32_t leftDespawn);
};