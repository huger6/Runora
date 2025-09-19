#include <orb.hpp>


// Public


Orb::Orb() : collected(false), orbSprite(getAuraOrbTexture()) {
    this->setHibbox(); // Debug
}

void Orb::drawHitbox(sf::RenderWindow& window) {
    for (auto orb = activeOrbs.begin(); orb != activeOrbs.end(); orb++) {
        hitbox.setPosition({static_cast<float>(orb->first.x), static_cast<float>(orb->first.y)});
        window.draw(hitbox);
    }
}

void Orb::update(bool collected) {
    this->collected = collected;
}


// Private


void Orb::setHibbox() {
    hitbox.setRadius(OrbConfigs::ORB_RADIUS);
    hitbox.setOrigin({hitbox.getLocalBounds().size.x / 2.0f, hitbox.getLocalBounds().size.y});
    
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineThickness(1.0f);
    hitbox.setOutlineColor(sf::Color::Red);
}

bool OrbManager::generateOrb(uint32_t tileX, uint32_t tileY) {
    if (tileY < MapConfigs::CHUNK_Y_AIR_VISIBLE * TileConfigs::CHUNK_HEIGHT + 1 && spawnOrb(OrbConfigs::ORB_SPAWN_PROBABILITY)) 
        return true;
}

bool OrbManager::spawnOrb(float probability) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0.0f, 1.0f);

    if (dist6(rng) < probability) return true;
}

void OrbManager::drawHitbox(sf::RenderWindow& window) {
    for (auto orb = activeOrbs.begin(); orb != activeOrbs.end(); orb++) {
        hitbox.setPosition({static_cast<float>(orb->first.x), static_cast<float>(orb->first.y)});
        window.draw(hitbox);
    }
}

void OrbManager::draw(sf::RenderWindow& window) {
    for (auto orb = activeOrbs.begin(); orb != activeOrbs.end(); orb++) {
        orbSprite.setPosition({static_cast<float>(orb->first.x), static_cast<float>(orb->first.y)});
        window.draw(orbSprite);
    }  
}