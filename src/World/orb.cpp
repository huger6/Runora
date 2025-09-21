#include "orb.hpp"
#include <fstream>
// JSON
#include <json.hpp>
// Tile
#include "tile.hpp"

// Public


Orb::Orb(sf::Vector2i position) : 
    collected(false), 
    position(position) {
        this->setHibbox(); // Debug
}

void Orb::drawHitbox(sf::RenderWindow& window) {
    hitbox.setPosition({static_cast<float>(position.x), static_cast<float>(position.y)});
    window.draw(hitbox);
}

bool Orb::intersectsHitbox(const sf::FloatRect& playerHitbox) {
    if (hitbox.getGlobalBounds().findIntersection(playerHitbox) == std::nullopt) return false;
    return true;
}

void Orb::setCollected() {
    this->collected = true;
}


OrbRegistry::OrbRegistry() = default;

void OrbRegistry::loadFromJSON(std::string filename)  {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Failed to open .json orbs file";
    }

    nlohmann::json data;
    file >> data;

    for (const auto& tile : data["tiles"]) {
        OrbInfo info;
        info.id = tile["id"].get<uint16_t>();
        info.name = tile["name"];
        info.x = tile["textureX"].get<uint16_t>();
        info.y = tile["textureY"].get<uint16_t>();
        info.textureRect = sf::IntRect({info.x, info.y}, {TileConfigs::TILE_SIZE * 2, TileConfigs::TILE_SIZE * 2});

        orbs[info.id] = info;
    }
}

const OrbInfo* OrbRegistry::getOrbInfo(const uint16_t id) const {
    auto orb = orbs.find(id);
    if (orb != orbs.end()) {
        OrbInfo* info = &orb->second;
        return info;
    }
    return nullptr;
}

std::optional<sf::IntRect> OrbRegistry::getTextureRect(const uint16_t id) const {
    const OrbInfo* orb = getOrbInfo(id);
    if (!orb) return std::nullopt;

    return orb->textureRect;
}




// Private


void Orb::setHibbox() {
    hitbox.setRadius(OrbConfigs::ORB_RADIUS);
    hitbox.setOrigin({hitbox.getLocalBounds().size.x / 2.0f, hitbox.getLocalBounds().size.y});
    
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineThickness(1.0f);
    hitbox.setOutlineColor(sf::Color::Red);
}


