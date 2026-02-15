#include "terrainGenerator.hpp"
#include <cmath>
// Getters
#include "Utils/getters.hpp"


// Public


TerrainGenerator::TerrainGenerator(TileRegistry& registry) : 
    activeRadiusX_front(ChunkConfigs::CHUNKS_FRONT_RADIUS), 
    activeRadiusX_back(ChunkConfigs::CHUNKS_BACK_RADIUS), 
    activeRadiusY(ChunkConfigs::CHUNKS_RADIUS_Y),
    registry(registry) {
        manager.loadSet(getTilesetPath());
}

void TerrainGenerator::update(const sf::Vector2f& playerPos) {
    updateChunks(playerPos);
}

void TerrainGenerator::draw(sf::RenderWindow& window) {
    for (auto& [coords, chunk] : map) {
        if (chunk->loaded) chunk->draw(window, manager);
    }
}

void TerrainGenerator::clear() {
    map.clear();
}

Chunk* TerrainGenerator::getChunk(const sf::Vector2f& playerCoords) {
    const sf::Vector2i chunkCoords = getChunkCoords(playerCoords);

    if (map.find(chunkCoords) != map.end()) {
        return map[chunkCoords].get(); // returns unique_ptr to the chunk
    }
    return nullptr;
}

/**
 * @brief Returns the TileId at a given world pixel position
 * 
 * @param worldX X position in pixels
 * @param worldY Y position in pixels
 * @return TileId at that position, or TileId::None if chunk not loaded
 */
TileId TerrainGenerator::getTileIdAtWorldPos(float worldX, float worldY) const {
    int tileX = static_cast<int>(floor(worldX / TileConfigs::TILE_SIZE));
    int tileY = static_cast<int>(floor(worldY / TileConfigs::TILE_SIZE));

    int chunkX = static_cast<int>(floor(static_cast<float>(tileX) / TileConfigs::CHUNK_WIDTH));
    int chunkY = static_cast<int>(floor(static_cast<float>(tileY) / TileConfigs::CHUNK_HEIGHT));

    sf::Vector2i chunkCoords(chunkX, chunkY);
    auto it = map.find(chunkCoords);
    if (it == map.end() || !it->second->loaded) return TileId::None;

    int localX = tileX - chunkX * TileConfigs::CHUNK_WIDTH;
    int localY = tileY - chunkY * TileConfigs::CHUNK_HEIGHT;

    if (localX < 0 || localX >= TileConfigs::CHUNK_WIDTH ||
        localY < 0 || localY >= TileConfigs::CHUNK_HEIGHT) return TileId::None;

    return it->second->getTileId(static_cast<uint16_t>(localX), static_cast<uint16_t>(localY));
}

/**
 * @brief Returns global bounds of the map where to spawn orbs
 * 
 * @param playerPos Current player's position
 * @return const std::vector<float>& 
 */
const std::vector<float> TerrainGenerator::getSpawningOrbsLimitsX(const sf::Vector2f playerPos) {
    std::vector<float> limits;

    limits.push_back(playerPos.x - activeRadiusX_back * TileConfigs::CHUNK_WIDTH * 1.5f);
    limits.push_back(playerPos.x + activeRadiusX_front * TileConfigs::CHUNK_WIDTH * 1.2f);

    return limits;
}

uint16_t TerrainGenerator::getSpawningOrbsLength() const {
    return static_cast<uint16_t>(ceil(abs(activeRadiusX_back * 1.5f - activeRadiusX_front * 1.2f)));
}


// Private


const sf::Vector2i TerrainGenerator::getChunkCoords(const sf::Vector2f& coords) const {
    return {static_cast<int>(floor(coords.x / (TileConfigs::CHUNK_WIDTH * TileConfigs::TILE_SIZE))), static_cast<int>(floor(coords.y / (TileConfigs::CHUNK_HEIGHT * TileConfigs::TILE_SIZE)))};
}

void TerrainGenerator::loadChunk(const sf::Vector2f& playerPos) {
    const sf::Vector2i chunkCoords = getChunkCoords(playerPos);
    auto chunk = map.find(chunkCoords);
    // Chunk does not exists
    if (chunk == map.end()) {
        auto [it, inserted] = map.try_emplace(chunkCoords, std::make_unique<Chunk>(chunkCoords, registry));
        if (inserted) it->second->load();
    }
}

void TerrainGenerator::unloadChunk(const sf::Vector2f& playerPos) {
    const sf::Vector2i chunkCoords = getChunkCoords(playerPos);
    auto chunk = map.find(chunkCoords);

    if (chunk != map.end()) {
        chunk->second->unload();

        map.erase(chunk);
    }
}

void TerrainGenerator::updateChunks(const sf::Vector2f& playerPos) {
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