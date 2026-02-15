#include "orbGenerator.hpp"
#include <random>
// Getter
#include "Utils/getters.hpp"

// Public


OrbGenerator::OrbGenerator(uint16_t arrayLength, OrbRegistry registry) :
    orbCounter(0),
    vertices(sf::PrimitiveType::Triangles, arrayLength * (MapConfigs::GROUND_Y - MapConfigs::JUMPING_MAX_Y) * 6),
    registry(registry) {
        texture.loadSet(getOrbPath());
    }

uint32_t OrbGenerator::getNumberOfOrbs() const {
    return orbCounter;
}

void OrbGenerator::update(const sf::Vector2f& playerPos, const sf::FloatRect& playerHitbox, const sf::View& camera) {
    uint32_t bottom = static_cast<uint32_t>(floor(MapConfigs::GROUND_Y / TileConfigs::TILE_SIZE));
    uint32_t top = static_cast<uint32_t>(floor(MapConfigs::JUMPING_MAX_Y / TileConfigs::TILE_SIZE));

    uint32_t leftDespawn = static_cast<uint32_t>(playerPos.x - (camera.getSize().x / 2.0f)) / TileConfigs::TILE_SIZE;
    uint32_t left = leftDespawn + OrbConfigs::ORB_SPAWN_X_OFFSET_IN_TILES;
    uint32_t right = left + OrbConfigs::ORB_SPAWN_X_SIZE_IN_TILES;

    for (auto orb = activeOrbs.begin(); orb != activeOrbs.end(); ) {
        const sf::Vector2i& coords = orb->first;
        // Check if orb is out of bounds
        if (static_cast<uint32_t>(coords.x) < leftDespawn) {
            orb = activeOrbs.erase(orb);
            continue;
        }
        
        // Check if it is collected
        if (orb->second->intersectsHitbox(playerHitbox)) {
            orbCounter++;
            orb = activeOrbs.erase(orb);
            continue;
        }

        orb++;
    }
    
    // Cleanup processed positions that are behind the player
    cleanupProcessedPositions(leftDespawn);

    if (activeOrbs.size() < OrbConfigs::MAX_NUMBER_ORBS_SPAWNED) {
        for (uint32_t x = left; x < right; ++x) {
            for (uint32_t y = top; y < bottom; ++y) {
                trySpawnOrb(x, y); // TODO
            }
        }
    }
    // std::cout << "Active orbs: " << activeOrbs.size() << "\n"; //debug
    buildVertexArray();
}

void OrbGenerator::draw(sf::RenderWindow& window) {
    sf::RenderStates states;
    states.texture = texture.getSetPtr();
    window.draw(vertices, states);
}

void OrbGenerator::drawHitbox(sf::RenderWindow& window) {
    for (auto orb = activeOrbs.begin(); orb != activeOrbs.end(); orb++) {
        orb->second->drawHitbox(window);
    }  
}

void OrbGenerator::reset() {
    activeOrbs.clear();
    processedPositions.clear();
    orbCounter = 0;
}


// Private


void OrbGenerator::buildVertexArray() {
    vertices.clear();
    vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    vertices.resize(activeOrbs.size() * 6);
    
    size_t orbIndex = 0;
    for (const auto& orb : activeOrbs) {
        const sf::Vector2i& position = orb.first;
        float size = 32.f;
        
        const auto& textureRect = registry.getTextureRect(1); // normal orb

        if (!textureRect.has_value()) {
            orbIndex++;
            continue;
        }

        const sf::IntRect& texRect = textureRect.value();

        sf::Vertex* triangle = &vertices[orbIndex * 6];

        // 0 ----- 1
        // |       |
        // |       |
        // 3-------2
        float x0 = static_cast<float>(position.x * TileConfigs::TILE_SIZE); // Top left X
        float y0 = static_cast<float>(position.y * TileConfigs::TILE_SIZE); // Top left Y
        float x1 = x0 + size; // Top right X / Bottom right X
        float y1 = y0 + size; // Bottom right Y / Bottom left Y

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

bool OrbGenerator::generateOrb(uint32_t /*tileX*/, uint32_t tileY) {
    if (tileY * TileConfigs::TILE_SIZE <= MapConfigs::GROUND_Y - TileConfigs::TILE_SIZE * 2
        && tileY * TileConfigs::TILE_SIZE >= MapConfigs::JUMPING_MAX_Y
        && spawnOrb(OrbConfigs::ORB_SPAWN_PROBABILITY)) {
            // std::cout << "Generated orb at tiles (" << tileX << "," << tileY << ")\n"; //debug
            return true;
    }
    return false;
}

bool OrbGenerator::spawnOrb(float probability) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    if (dist(rng) < probability) return true;
    return false;
}

void OrbGenerator::trySpawnOrb(uint32_t tileX, uint32_t tileY) {
    sf::Vector2i coords(tileX, tileY);
    
    // Skip if this position was already processed (tried to spawn)
    if (processedPositions.contains(coords)) return;
    
    // Mark position as processed
    processedPositions.insert(coords);
    
    // Try to generate orb based on probability
    if (!generateOrb(tileX, tileY)) return;

    // Don't allow the orb to spawn on an existing orb
    if (activeOrbs.contains(coords)) return;

    auto orb = std::make_unique<Orb>(coords);

    // Insert on the umap
    activeOrbs.emplace(coords, std::move(orb));
}

void OrbGenerator::cleanupProcessedPositions(uint32_t leftDespawn) {
    for (auto it = processedPositions.begin(); it != processedPositions.end(); ) {
        if (static_cast<uint32_t>(it->x) < leftDespawn) {
            it = processedPositions.erase(it);
        } else {
            ++it;
        }
    }
}