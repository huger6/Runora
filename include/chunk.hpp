#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <tile.hpp>
#include <texture.hpp>
#include <map.hpp>


namespace ChunkConfigs {
    constexpr uint16_t MAX_CHUNKS_X = 20;
    constexpr uint16_t MAX_CHUNKS_Y = 5;
    constexpr int16_t CHUNKS_FRONT_RADIUS = 4;
    constexpr int16_t CHUNKS_BACK_RADIUS = 1;
    constexpr int16_t CHUNKS_RADIUS_Y = 3;
}


class Chunk {
    protected:
        TileId tiles[TileConfigs::CHUNK_WIDTH][TileConfigs::CHUNK_HEIGHT];
        sf::VertexArray vertices;
        TileRegistry& registry;
        sf::IntRect bbox;
        bool needsUpdate;

        /**
         * @brief Generates terrain for the chunk
         * 
         * @param tileX Width in tiles
         * @param tileY Height in tiles
         * @return TileId 
         */
        TileId proceduralGenerator(uint32_t tileX, uint32_t tileY) {
            // if (chunkCoords.y * TileConfigs::TILE_SIZE < MapConfigs::CHUNK_Y_CLOUDS_VISIBLE && generateClouds(tileX, tileY)) return TileId::Clouds;
            if (tileY <= 104 && tileY >= 98 && generateClouds(tileX, tileY, 2)) {
                // std::cout << "TileY:" << tileY;
                return TileId::Clouds;
            }
            if (tileY <= 97 && generateClouds(tileX, tileY, 4)) {
                // std::cout << "TileY:" << tileY;
                return TileId::Clouds;
            }
            if (chunkCoords.y < MapConfigs::CHUNK_Y_AIR_VISIBLE) return TileId::Air;
            else return TileId::Brick;
            // if (tileY <= TileConfigs::GROUND_HEIGTH * TileConfigs::TILE_SIZE) return TileId::Air;
            // else return TileId::Brick;
        }

        const bool generateClouds(uint32_t tileX, uint32_t tileY, uint16_t prob) const {
            uint32_t seed = 0x27d4eb2d;
            uint32_t hash = tileX * 374761393u + tileY * 668265263u; 
            hash = (hash ^ (hash >> 13)) * seed;
            hash = (hash ^ (hash >> 15));

            // 4% prob
            return (hash % 100) < (prob % 100);
        }
    public:
        sf::Vector2i chunkCoords; // floor(tileX / CHUNK_WIDTH) ...
        bool loaded;
    
        Chunk(sf::Vector2i chunkCoordinates, TileRegistry& registry) : vertices(sf::PrimitiveType::Triangles, TileConfigs::CHUNK_WIDTH * TileConfigs::CHUNK_HEIGHT * 6), registry(registry),
            needsUpdate(true), chunkCoords(chunkCoordinates), loaded(false) {
                for (int y = 0; y < TileConfigs::CHUNK_HEIGHT; y++) {
                    for (int x = 0; x < TileConfigs::CHUNK_WIDTH; x++) {
                        tiles[x][y] = TileId::None;
                    }
                }
                bbox = sf::IntRect({chunkCoords.x * TileConfigs::CHUNK_WIDTH_IN_PX, chunkCoords.y * TileConfigs::CHUNK_HEIGHT_IN_PX},
                                    {TileConfigs::CHUNK_WIDTH_IN_PX, TileConfigs::CHUNK_HEIGHT_IN_PX});
        }

        const sf::Vector2i& getChunkCoords() const {
            return chunkCoords;
        }

        void load() {
            for (int y = 0; y < TileConfigs::CHUNK_HEIGHT; y++) {
                for (int x = 0; x < TileConfigs::CHUNK_WIDTH; x++) {
                    uint32_t tileX = chunkCoords.x * TileConfigs::CHUNK_WIDTH + x;
                    uint32_t tileY = chunkCoords.y * TileConfigs::CHUNK_HEIGHT + y;
                    tiles[x][y] = proceduralGenerator(tileX, tileY);
                }
            }
            buildVertexArray();
            loaded = true;
            needsUpdate = false;
        }

        void unload() {
            loaded = false;
        }

        void buildVertexArray() {
            for (int y = 0; y < TileConfigs::CHUNK_HEIGHT; y++) {
                for (int x = 0; x < TileConfigs::CHUNK_WIDTH; x++) {
                    TileId id = tiles[x][y];

                    if (id == TileId::None) continue;

                    const auto& textureRect = registry.getTextureRect(id);

                    if (!textureRect.has_value()) continue;

                    const sf::IntRect& texRect = textureRect.value();

                    sf::Vertex* triangle = &vertices[(x + y * TileConfigs::CHUNK_WIDTH) * 6];

                    // 0 ----- 1
                    // |       |
                    // |       |
                    // 3-------2
                    float x0 = static_cast<float>(chunkCoords.x * TileConfigs::CHUNK_WIDTH * TileConfigs::TILE_SIZE + x * TileConfigs::TILE_SIZE); // Top left X
                    float y0 = static_cast<float>(chunkCoords.y * TileConfigs::CHUNK_HEIGHT * TileConfigs::TILE_SIZE + y * TileConfigs::TILE_SIZE); // Top left Y
                    float x1 = x0 + TileConfigs::TILE_SIZE; // Top right X / Bottom right X
                    float y1 = y0 + TileConfigs::TILE_SIZE; // Bottom right Y / Bottom left Y

                    float tx0 = static_cast<float>(texRect.position.x); // Top left
                    float ty0 = static_cast<float>(texRect.position.y); // Top right
                    float tx1 = static_cast<float>(texRect.position.x + texRect.size.x); // Bottom right
                    float ty1 = static_cast<float>(texRect.position.y + texRect.size.y); // Bottom left

                    // Triangle 0 - 1 - 2
                    triangle[0].position = {x0, y0}; // 0
                    triangle[1].position = {x1, y0}; // 1
                    triangle[2].position = {x1, y1}; // 2
                    // Triangle 0 - 1 - 3
                    triangle[3].position = {x0, y0}; // 0
                    triangle[4].position = {x1, y1}; // 2
                    triangle[5].position = {x0, y1}; // 3

                    // Tileset coords to be used

                    // Triangle 0 - 1 - 2
                    triangle[0].texCoords = {tx0, ty0};
                    triangle[1].texCoords = {tx1, ty0};
                    triangle[2].texCoords = {tx1, ty1};

                    // Triangle 0 - 2 - 3
                    triangle[3].texCoords = {tx0, ty0};
                    triangle[4].texCoords = {tx1, ty1};
                    triangle[5].texCoords = {tx0, ty1};

                }
            }
        }

        TileId getTileId(uint16_t tileX, uint16_t tileY) const {
            return tiles[tileX][tileY];
        }

        void setTile(uint16_t tileX, uint16_t tileY, TileId id) {
            tiles[tileX][tileY] = id;
            needsUpdate = true;
        }

        bool contains(uint16_t pixelX, uint16_t pixelY) const {
            if (bbox.contains({pixelX, pixelY})) return true;
            return false;
        }

        void draw(sf::RenderWindow& window, TextureManager& texManager) {
            sf::RenderStates states;
            states.texture = texManager.getTilesetPtr();
            window.draw(vertices, states);
            // drawBorder(window);
        }

        void drawBorder(sf::RenderWindow& window) const {
            sf::RectangleShape border;
            border.setPosition({static_cast<float>(bbox.position.x), static_cast<float>(bbox.position.y)});
            border.setSize({static_cast<float>(bbox.size.x), static_cast<float>(bbox.size.y)});
            border.setFillColor(sf::Color::Transparent);
            border.setOutlineColor(sf::Color::Red);
            border.setOutlineThickness(2.f);

            sf::Text coords(getFont());
            std::string xy = std::format("({},{})", chunkCoords.x, chunkCoords.y);
            coords.setString(xy);
            coords.setCharacterSize(50);
            coords.setFillColor(sf::Color::Red);
            coords.setOrigin(coords.getLocalBounds().getCenter());
            coords.setPosition({bbox.position.x + bbox.size.x / 2.0f, bbox.position.y + bbox.size.y / 2.0f});
            window.draw(border);
            window.draw(coords);
        }
};