#include "hud.hpp"


// Public


HUD::HUD() = default;

sf::Time HUD::getElapsed() const { return timer.getElapsed(); }

uint32_t HUD::getTotalOrbs() const { return orbs.getTotalOrbs(); }

void HUD::resetTimer() {
    timer.reset();
}

void HUD::update(uint32_t orbsToAdd, bool playing) {
    timer.update(playing);
    orbs.update(orbsToAdd);
}

void HUD::draw(sf::RenderWindow& window, sf::View& camera) {
    timer.draw(window, camera);
    orbs.draw(window, camera);
}
