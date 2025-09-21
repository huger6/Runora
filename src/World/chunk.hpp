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

        bool generateClouds(uint32_t tileX, uint32_t tileY, uint16_t prob) const;
};