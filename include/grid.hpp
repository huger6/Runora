#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <chunk.hpp>
#include <texture.hpp>
#include <getters.hpp>


struct Vector2iHash {
    std::size_t operator()(const sf::Vector2i& p) const noexcept {
        std::size_t h1 = std::hash<int>{}(p.x);
        std::size_t h2 = std::hash<int>{}(p.y);
        return h1 ^ (h2 << 1);
    }
};


class Grid {
    private:
        std::unordered_map<sf::Vector2i, std::unique_ptr<Chunk>, Vector2iHash> map;
        TextureManager manager;
        TileRegistry& registry;

        const sf::Vector2i getChunkCoords(const sf::Vector2f& coords) const {
            return {static_cast<int>(floor(coords.x / (TileConfigs::CHUNK_WIDTH * TileConfigs::TILE_SIZE))), static_cast<int>(floor(coords.y / (TileConfigs::CHUNK_HEIGHT * TileConfigs::TILE_SIZE)))};
        }
    public:
        uint16_t activeRadiusX_front; // Number of chunks to have loaded in front of the player
        uint16_t activeRadiusX_back;
        uint16_t activeRadiusY;

        Grid(TileRegistry& registry) : registry(registry), activeRadiusX_front(ChunkConfigs::CHUNKS_FRONT_RADIUS), activeRadiusX_back(ChunkConfigs::CHUNKS_BACK_RADIUS), activeRadiusY(ChunkConfigs::CHUNKS_RADIUS_Y) {
            manager.loadTileset(getTilesetPath());
        }

        void loadChunk(const sf::Vector2f& playerPos) {
            const sf::Vector2i chunkCoords = getChunkCoords(playerPos);
            auto chunk = map.find(chunkCoords);
            // Chunk does not exists
            if (chunk == map.end()) {
                auto [it, inserted] = map.try_emplace(chunkCoords, std::make_unique<Chunk>(chunkCoords, registry));
                if (inserted) it->second->load();
            }
        }

        void unloadChunk(const sf::Vector2f& playerPos) {
            const sf::Vector2i chunkCoords = getChunkCoords(playerPos);
            auto chunk = map.find(chunkCoords);

            if (chunk != map.end()) {
                chunk->second->unload();

                map.erase(chunk);
            }
        }

        void updateChunks(const sf::Vector2f& playerPos) {
            const sf::Vector2i chunkCoords = getChunkCoords(playerPos);

            // Unload inactive chunks
            for (auto chunk = map.begin(); chunk != map.end(); ) {
                const sf::Vector2i& coords = chunk->first;

                uint32_t dx = std::abs(coords.x - chunkCoords.x);
                uint32_t dy = std::abs(coords.y - chunkCoords.y);

                // Distance behind player (X axis)
                if (chunkCoords.x > coords.x + activeRadiusX_back) {
                    chunk = map.erase(chunk);
                }
                // Distance in front of player (X axis)
                else if (dx > activeRadiusX_front) {
                    chunk = map.erase(chunk);
                }
                // Chunk is too far away (Y axis)
                else if (dy > activeRadiusY) {
                    chunk = map.erase(chunk);
                }
                else {
                    ++chunk;
                }
            }

            // Load upcoming close chunks
            uint32_t upperLimitX = chunkCoords.x + activeRadiusX_front;
            uint32_t upperLimitY = chunkCoords.y + activeRadiusY;
            for (uint32_t x = chunkCoords.x - activeRadiusX_front; x <= upperLimitX; x++) {
                for (uint32_t y = chunkCoords.y - activeRadiusY; y <= upperLimitY; y++) {
                    sf::Vector2i coords(x, y);

                    auto [it, inserted] = map.try_emplace(coords, std::make_unique<Chunk>(coords, registry));
                    if (inserted) it->second->load(); // chunk->load()
                }
            }
        }

        void draw(sf::RenderWindow& window) {
            for (auto& [coords, chunk] : map) {
                if (chunk->loaded) chunk->draw(window, manager);
            }
        }

        Chunk* getChunk(const sf::Vector2f& playerCoords) {
            const sf::Vector2i chunkCoords = getChunkCoords(playerCoords);

            if (map.find(chunkCoords) != map.end()) {
                return map[chunkCoords].get(); // returns unique_ptr to the chunk
            }
            return nullptr;
        }

        void clear() {
            map.clear();
        }
};