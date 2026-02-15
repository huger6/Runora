#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
// SFML
#include <SFML/Graphics.hpp>
// World
#include "World/chunk.hpp"
// Texture
#include "Graphics/texture.hpp"
// Utils
#include "Utils/getters.hpp"


class TerrainGenerator {
    public:
        uint16_t activeRadiusX_front; // Number of chunks to have loaded in front of the player
        uint16_t activeRadiusX_back;
        uint16_t activeRadiusY;

        TerrainGenerator(TileRegistry& registry);

        void update(const sf::Vector2f& playerPos);
        void draw(sf::RenderWindow& window);

        void clear();
        Chunk* getChunk(const sf::Vector2f& playerCoords);
        TileId getTileIdAtWorldPos(float worldX, float worldY) const;
        const std::vector<float> getSpawningOrbsLimitsX(const sf::Vector2f playerPos);
        uint16_t getSpawningOrbsLength() const;
    private:
        std::unordered_map<sf::Vector2i, std::unique_ptr<Chunk>, Vector2iHash> map;
        TextureManager manager;
        TileRegistry& registry;

        const sf::Vector2i getChunkCoords(const sf::Vector2f& coords) const;

        void loadChunk(const sf::Vector2f& playerPos);
        void unloadChunk(const sf::Vector2f& playerPos);
        void updateChunks(const sf::Vector2f& playerPos);
};