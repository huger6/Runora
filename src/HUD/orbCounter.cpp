#include "orbCounter.hpp"
// Getters
#include "Utils/getters.hpp"


// Public


OrbCounter::OrbCounter() : text(getFont()) {
    text.setFillColor(sf::Color::Magenta);
}

uint32_t OrbCounter::getTotalOrbs() const { return orbs; }

void OrbCounter::update(uint32_t orbsToAdd) {
    orbs += orbsToAdd;
}

void OrbCounter::draw(sf::RenderWindow& window, sf::View& camera) {
    setCounterPosition(camera);
    text.setString(std::to_string(orbs));
    window.draw(text);
}


// Private


void OrbCounter::setCounterPosition(sf::View& view) {
    sf::Vector2f center = view.getCenter();
    sf::Vector2f size = view.getSize();
    float left = center.x - size.x / 2.0f;
    float top = center.y - size.y / 2.0f;
    text.setPosition({left + 200.0f, top + 20.0f});
}
