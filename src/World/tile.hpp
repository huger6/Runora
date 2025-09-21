#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
// SFML
#include <SFML/Graphics.hpp>

namespace TileConfigs {
    constexpr int16_t TILE_SIZE = 16;
    constexpr int16_t CHUNK_WIDTH = 80; // 1 chunk at 720p (80 tiles width per chunk)
    constexpr int16_t CHUNK_HEIGHT = 20; // 3 chunks at 720p (20 tiles height per chunk)
    constexpr int16_t CHUNK_WIDTH_IN_PX = CHUNK_WIDTH * TILE_SIZE;
    constexpr int16_t CHUNK_HEIGHT_IN_PX = CHUNK_HEIGHT * TILE_SIZE;
    constexpr int16_t GROUND_HEIGTH = 19; // 1 whole chunk
    constexpr int16_t GROUND_VISIBLE = 15; // 15 tiles visible to the player
}

enum class TileType {
    None,
    Ground,
    Air, 
    Water,
    Lava,
    Obstacle      
};

enum class CollisionType {
    None, 
    Solid,
    Liquid
};

enum class TileId : uint16_t {
    None = 0,
    Air = 1,
    Clouds = 2,
    Brick = 3,
    Water = 4,
    Waves = 5,
    Lava = 6
};

struct Tile {
    uint16_t id;
    TileType type; 
    bool collidable;
    // variant, lightlevel, biome
};

struct TileInfo {
    uint16_t id;
    std::string name;
    TileType type;
    uint16_t x, y;
    CollisionType collision;
    sf::IntRect textureRect;
};

class TileRegistry {
    public:
        TileRegistry();

        void loadFromJSON(std::string filename);

        const TileInfo* getTileInfo(const TileId id) const;
        std::optional<sf::IntRect> getTextureRect(const TileId id) const;
        bool isCollisionType(const TileId id, CollisionType col) const;
    private:
        inline static std::unordered_map<uint16_t, TileInfo> tiles;

        TileType stringToTileType(const std::string& str);
        CollisionType stringToCollisionType(const std::string& str);
        uint16_t getInternalId(const TileId id) const;
};