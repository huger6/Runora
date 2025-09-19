#include <hud.hpp>


// Public


HUD::HUD() = default;

sf::Time HUD::getElapsed() const { return timer.getElapsed(); }

uint32_t HUD::getTotalOrbs() const { return orbs.getTotalOrbs(); }

void HUD::update(uint32_t orbsToAdd) {
    orbs.update(orbsToAdd);
}

void HUD::draw(sf::RenderWindow& window, sf::View& camera) {
    timer.draw(window, camera);
    orbs.draw(window, camera);
}
