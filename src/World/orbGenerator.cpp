#include "orbGenerator.hpp"
#include <random>
// Getter
#include "Utils/getters.hpp"

// Public


OrbGenerator::OrbGenerator(uint16_t arrayLength) :
    orbCounter(0),
    vertices(sf::PrimitiveType::Triangles, arrayLength * (MapConfigs::GROUND_Y - MapConfigs::JUMPING_MAX_Y) * 6) {
        texture.loadSet(getOrbPath());
        registry.loadFromJSON(getOrbsetJSONPath());
    }

uint32_t OrbGenerator::getNumberOfOrbs() const {
    return orbCounter;
}

// void OrbGenerator::drawHitbox(sf::RenderWindow& window) {
//     for (auto orb = activeOrbs.begin(); orb != activeOrbs.end(); orb++) {
//         hitbox.setPosition({static_cast<float>(orb->first.x), static_cast<float>(orb->first.y)});
//         window.draw(hitbox);
//     }
// }



void OrbGenerator::update(const std::vector<float>& limitsX, const sf::FloatRect& playerHitbox, const sf::View& camera) {
    float bottom = MapConfigs::GROUND_Y / TileConfigs::TILE_SIZE;
    float top = MapConfigs::JUMPING_MAX_Y / TileConfigs::TILE_SIZE;

    uint32_t left = static_cast<uint32_t>(limitsX[0]) / TileConfigs::TILE_SIZE;
    uint32_t right = static_cast<uint32_t>(limitsX[1]) / TileConfigs::TILE_SIZE;

    for (auto orb = activeOrbs.begin(); orb != activeOrbs.end(); orb++) {
        const sf::Vector2i& coords = orb->first;
        // Check if orb is out of bounds
        if (coords.x < left || coords.x > right) {
            activeOrbs.erase(coords);
            continue;
        }

        // Check if it is collected
        if (orb->second->intersectsHitbox(playerHitbox)) {
            orbCounter++;
            activeOrbs.erase(coords);
            continue;
        }
    }  

    for (int x = left; x < right; ++x) {
        for (int y = top; y < bottom; ++y) {
            trySpawnOrb(x, y); // it might spawn on the player!!
        }
    }

}

void OrbGenerator::draw(sf::RenderWindow& window) {
    sf::RenderStates states;
    states.texture = texture.getSetPtr();
    window.draw(vertices, states);
}


// Private


void OrbGenerator::buildVertexArray() {
    vertices.clear();
    
    size_t orbIndex = 0;
    for (const auto& orb : activeOrbs) {
        const sf::Vector2i& position = orb.first;
        float size = 32.f;
        
        const auto& textureRect = registry.getTextureRect(1); // normal orb

        if (!textureRect.has_value()) continue;

        const sf::IntRect& texRect = textureRect.value();

        sf::Vertex* triangle = &vertices[orbIndex * 6];

        // 0 ----- 1
        // |       |
        // |       |
        // 3-------2
        float x0 = static_cast<float>(position.x ); // Top left X
        float y0 = static_cast<float>(position.y); // Top left Y
        float x1 = x0 + TileConfigs::TILE_SIZE * 2; // Top right X / Bottom right X
        float y1 = y0 + TileConfigs::TILE_SIZE * 2; // Bottom right Y / Bottom left Y

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

        // OrbSet coords to be used

        // Triangle 0 - 1 - 2
        triangle[0].texCoords = {tx0, ty0};
        triangle[1].texCoords = {tx1, ty0};
        triangle[2].texCoords = {tx1, ty1};

        // Triangle 0 - 2 - 3
        triangle[3].texCoords = {tx0, ty0};
        triangle[4].texCoords = {tx1, ty1};
        triangle[5].texCoords = {tx0, ty1};

        // Increment
        orbIndex++;
    }
}

bool OrbGenerator::generateOrb(uint32_t tileX, uint32_t tileY) {
    if (tileY < MapConfigs::GROUND_Y / TileConfigs::TILE_SIZE + 1 
        && tileY > MapConfigs::JUMPING_MAX_Y / TileConfigs::TILE_SIZE 
        && spawnOrb(OrbConfigs::ORB_SPAWN_PROBABILITY)) {
        return true;
    }
    return false;
}

bool OrbGenerator::spawnOrb(float probability) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0.0f, 1.0f);

    if (dist6(rng) < probability) return true;
    return false;
}

void OrbGenerator::trySpawnOrb(uint32_t tileX, uint32_t tileY) {
    if (!generateOrb(tileX, tileY)) return;

    sf::Vector2i coords(tileX * TileConfigs::TILE_SIZE,
                        tileY * TileConfigs::TILE_SIZE);
    
    // Don't allow the orb to spawn on the player
    if (activeOrbs.contains(coords)) return;

    // std::cout << "Spawning orb now" << std::endl;
    auto orb = std::make_unique<Orb>(coords);

    // Insert on the umap
    activeOrbs.emplace(coords, std::move(orb));
}