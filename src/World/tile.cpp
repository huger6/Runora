#include "tile.hpp"
#include <fstream>
// JSON
#include <json.hpp> // from https://github.com/nlohmann/json
// Utils
#include "Utils/getters.hpp"
#include "Utils/utils.hpp"


// Public


TileRegistry::TileRegistry() = default;

void TileRegistry::loadFromJSON(std::string filename)  {
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

const TileInfo* TileRegistry::getTileInfo(const TileId id) const {
    uint16_t int_id = getInternalId(id);
    auto tile = tiles.find(int_id);
    if (tile != tiles.end()) {
        TileInfo* info = &tile->second;
        return info;
    }
    return nullptr;
}

std::optional<sf::IntRect> TileRegistry::getTextureRect(const TileId id) const {
    const TileInfo* tile = getTileInfo(id);
    if (!tile) return std::nullopt;

    return tile->textureRect;
}

bool TileRegistry::isCollisionType(const TileId id, CollisionType col) const {
    const TileInfo* tile = getTileInfo(id);
    if (tile && tile->collision == col) return true;
    return false;
}


// Private


TileType TileRegistry::stringToTileType(const std::string& str) {
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

CollisionType TileRegistry::stringToCollisionType(const std::string& str) {
    if (str.empty()) return CollisionType::None;
    std::string type = capitalize(str);

    if (type == "None") return CollisionType::None;
    if (type == "Solid") return CollisionType::Solid;
    if (type == "Liquid") return CollisionType::Liquid;

    return CollisionType::None;
}

uint16_t TileRegistry::getInternalId(const TileId id) const {
    return static_cast<uint16_t>(id);
}