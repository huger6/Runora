#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <Utils/getters.hpp>
#include <json.hpp> // from https://github.com/nlohmann/json
#include <Utils/utils.hpp>

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
    private:
        inline static std::unordered_map<uint16_t, TileInfo> tiles;

        TileType stringToTileType(const std::string& str) {
            if (str.empty()) return TileType::None;
            std::string type = capitalize(str);

            if (type == "None") return TileType::None;
            if (type == "Ground") return TileType::Ground;
            if (type == "Air") return TileType::Air;
            if (type == "Water") return TileType::Water;
            if (type == "Lava") return TileType::Lava;
            if (type == "Obstacle") return TileType::Obstacle;

            return TileType::None;
        }

        CollisionType stringToCollisionType(const std::string& str) {
            if (str.empty()) return CollisionType::None;
            std::string type = capitalize(str);

            if (type == "None") return CollisionType::None;
            if (type == "Solid") return CollisionType::Solid;
            if (type == "Liquid") return CollisionType::Liquid;

            return CollisionType::None;
        }

        uint16_t getInternalId(const TileId id) const {
            return static_cast<uint16_t>(id);
        }

    public:
        TileRegistry() = default;

        void loadFromJSON(std::string filename)  {
            std::ifstream file(filename);
            if (!file.is_open()) {
                std::cout << "Failed to open .json tileset file";
            }

            nlohmann::json data;
            file >> data;

            for (const auto& tile : data["tiles"]) {
                TileInfo info;
                info.id = tile["id"].get<uint16_t>();
                info.name = tile["name"];
                info.type = stringToTileType(tile["type"]);
                info.x = tile["textureX"].get<uint16_t>();
                info.y = tile["textureY"].get<uint16_t>();
                info.collision = stringToCollisionType(tile["collision"]);
                info.textureRect = sf::IntRect({info.x, info.y}, {TileConfigs::TILE_SIZE, TileConfigs::TILE_SIZE});

                tiles[info.id] = info;
            }
        }

        const TileInfo* getTileInfo(const TileId id) const {
            uint16_t int_id = getInternalId(id);
            auto tile = tiles.find(int_id);
            if (tile != tiles.end()) {
                TileInfo* info = &tile->second;
                return info;
            }
            return nullptr;
        }

        std::optional<sf::IntRect> getTextureRect(const TileId id) const {
            const TileInfo* tile = getTileInfo(id);
            if (!tile) return std::nullopt;

            return tile->textureRect;
        }

        bool isCollisionType(const TileId id, CollisionType col) const {
            const TileInfo* tile = getTileInfo(id);
            if (tile && tile->collision == col) return true;
            return false;
        }
};