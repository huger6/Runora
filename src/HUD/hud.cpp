#include "hud.hpp"


// Public


HUD::HUD() = default;

sf::Time HUD::getElapsed() const { return timer.getElapsed(); }

uint32_t HUD::getTotalOrbs() const { return orbs.getTotalOrbs(); }

void HUD::resetTimer() {
    timer.reset();
}

void HUD::update(uint32_t orbsTotal, bool playing) {
    timer.update(playing);
    orbs.update(orbsTotal);
}

void HUD::draw(sf::RenderWindow& window, sf::View& camera) {
    timer.draw(window, camera);
    orbs.draw(window, camera);
}
