#pragma once

#include <iostream>
// SFML
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
// World
#include "World/tile.hpp"
#include "World/map.hpp"
// Graphics
#include "Graphics/texture.hpp"


namespace ChunkConfigs {   
    constexpr uint16_t MAX_CHUNKS_X = 20;
    constexpr uint16_t MAX_CHUNKS_Y = 5;
    constexpr int16_t CHUNKS_FRONT_RADIUS = 4;
    constexpr int16_t CHUNKS_BACK_RADIUS = 1;
    constexpr int16_t CHUNKS_RADIUS_Y = 3;
    constexpr uint16_t TILE_GENERATE_CLOUDS_2 = 104;
    constexpr uint16_t TILE_GENERATE_CLOUDS_4 = 97;
}

namespace HazardConfigs {
    constexpr uint32_t ZONE_SIZE = 30;           // Tiles per potential hazard zone (smaller = more frequent)
    constexpr uint32_t HAZARD_CHANCE = 70;       // Percentage chance per zone to spawn a hazard
    constexpr uint32_t MIN_LENGTH = 3;           // Minimum hazard segment length (very easy hop)
    constexpr uint32_t MAX_LENGTH = 16;          // Maximum hazard segment length (hard, needs double jump)
    constexpr uint32_t DEPTH = 3;                // How many tile rows deep the hazard pool extends
    constexpr uint32_t SAFE_ZONE_X = 370;        // No hazards before this tile X (protects spawn area)
    constexpr uint32_t ZONE_PADDING = 3;         // Min gap from zone edges (prevents adjacent different-type traps)
}


class Chunk {
    public:
        sf::Vector2i chunkCoords; // floor(tileX / CHUNK_WIDTH) ...
        bool loaded;
    
        Chunk(sf::Vector2i chunkCoordinates, TileRegistry& registry);

        const sf::Vector2i& getChunkCoords() const;

        void load();
        void unload();
        void buildVertexArray();

        TileId getTileId(uint16_t tileX, uint16_t tileY) const;
        void setTile(uint16_t tileX, uint16_t tileY, TileId id);

        bool contains(uint16_t pixelX, uint16_t pixelY) const;

        void draw(sf::RenderWindow& window, TextureManager& texManager);

        void setBorder();
        void drawBorder(sf::RenderWindow& window) const;
    protected:
        TileId tiles[TileConfigs::CHUNK_WIDTH][TileConfigs::CHUNK_HEIGHT];
        sf::VertexArray vertices;
        TileRegistry& registry;
        sf::IntRect bbox;
        sf::RectangleShape border;
        sf::Text coordsText;
        bool needsUpdate;

        TileId proceduralGenerator(uint32_t tileX, uint32_t tileY);
        TileId generateHazard(uint32_t tileX, uint32_t tileY, uint32_t groundStartY);

        bool generateClouds(uint32_t tileX, uint32_t tileY, uint16_t prob) const;
        static uint32_t hazardHash(uint32_t seed);
};