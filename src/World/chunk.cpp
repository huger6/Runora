#include <chunk.hpp>


// Public


Chunk::Chunk(sf::Vector2i chunkCoordinates, TileRegistry& registry) : 
    vertices(sf::PrimitiveType::Triangles, TileConfigs::CHUNK_WIDTH * TileConfigs::CHUNK_HEIGHT * 6),
    registry(registry), 
    coordsText(getFont()), needsUpdate(true), chunkCoords(chunkCoordinates),
    loaded(false) {
        for (int y = 0; y < TileConfigs::CHUNK_HEIGHT; y++) {
            for (int x = 0; x < TileConfigs::CHUNK_WIDTH; x++) {
                tiles[x][y] = TileId::None;
            }
        }
        bbox = sf::IntRect({chunkCoords.x * TileConfigs::CHUNK_WIDTH_IN_PX, chunkCoords.y * TileConfigs::CHUNK_HEIGHT_IN_PX},
                            {TileConfigs::CHUNK_WIDTH_IN_PX, TileConfigs::CHUNK_HEIGHT_IN_PX});
}

const sf::Vector2i& Chunk::getChunkCoords() const {
    return chunkCoords;
}

void Chunk::load() {
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

void Chunk::unload() {
    loaded = false;
}

void Chunk::buildVertexArray() {
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

TileId Chunk::getTileId(uint16_t tileX, uint16_t tileY) const {
    return tiles[tileX][tileY];
}

void Chunk::setTile(uint16_t tileX, uint16_t tileY, TileId id) {
    tiles[tileX][tileY] = id;
    needsUpdate = true;
}

bool Chunk::contains(uint16_t pixelX, uint16_t pixelY) const {
    if (bbox.contains({pixelX, pixelY})) return true;
    return false;
}

void Chunk::draw(sf::RenderWindow& window, TextureManager& texManager) {
    sf::RenderStates states;
    states.texture = texManager.getTilesetPtr();
    window.draw(vertices, states);
    // drawBorder(window); // debug
}

void Chunk::setBorder() {
    border.setPosition({static_cast<float>(bbox.position.x), static_cast<float>(bbox.position.y)});
    border.setSize({static_cast<float>(bbox.size.x), static_cast<float>(bbox.size.y)});
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::Red);
    border.setOutlineThickness(2.f);

    std::string xy = std::format("({},{})", chunkCoords.x, chunkCoords.y);
    coordsText.setString(xy);
    coordsText.setCharacterSize(50);
    coordsText.setFillColor(sf::Color::Red);
    coordsText.setOrigin(coordsText.getLocalBounds().getCenter());
    coordsText.setPosition({bbox.position.x + bbox.size.x / 2.0f, bbox.position.y + bbox.size.y / 2.0f});
}

void Chunk::drawBorder(sf::RenderWindow& window) const {
    window.draw(border);
    window.draw(coordsText);
}


// Protected


/**
 * @brief Generates terrain for the chunk
 * 
 * @param tileX Width in tiles
 * @param tileY Height in tiles
 * @return TileId 
 */
TileId Chunk::proceduralGenerator(uint32_t tileX, uint32_t tileY) {
    // Air generation
    if (tileY <= ChunkConfigs::TILE_GENERATE_CLOUDS_2 && tileY >= 98 && generateClouds(tileX, tileY, 2)) {
        return TileId::Clouds;
    }
    if (tileY <= ChunkConfigs::TILE_GENERATE_CLOUDS_4 && generateClouds(tileX, tileY, 4)) {
        return TileId::Clouds;
    }
    if (tileY < MapConfigs::CHUNK_Y_AIR_VISIBLE * TileConfigs::CHUNK_HEIGHT + 1) return TileId::Air;
    // Ground generation
    
    return TileId::Brick;
}

bool Chunk::generateClouds(uint32_t tileX, uint32_t tileY, uint16_t prob) const {
    uint32_t seed = 0x27d4eb2d;
    uint32_t hash = tileX * 374761393u + tileY * 668265263u; 
    hash = (hash ^ (hash >> 13)) * seed;
    hash = (hash ^ (hash >> 15));

    // 4% prob
    return (hash % 100) < (prob % 100);
}