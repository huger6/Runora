#include "time.hpp"
// Utils
#include "Utils/getters.hpp"


// Public


GameTimer::GameTimer() : time(getFont()) {
    clock.start();

    time.setFillColor(sf::Color::Red);
    time.setOrigin(time.getLocalBounds().getCenter());
}

// Get time elapsed until now
sf::Time GameTimer::getElapsed() const { return clock.getElapsedTime(); }


// Private


void GameTimer::setTimerPosition(sf::View& camera) {
    sf::Vector2f center = camera.getCenter();
    sf::Vector2f size = camera.getSize();
    float left = center.x - size.x / 2.0f;
    float top = center.y - size.y / 2.0f;
    time.setPosition({left + 30.0f, top + 20.0f});
}

void GameTimer::start() {
    clock.restart();
}

void GameTimer::pause() {
    if (clock.isRunning()) clock.stop();
}

void GameTimer::resume() {
    if (!clock.isRunning()) clock.start();
}

void GameTimer::reset() {
    clock.reset();
}

inline std::string GameTimer::getElapsedFormatted() {
    sf::Time timeElapsed = getElapsed();
    int32_t ms = timeElapsed.asMilliseconds();
    int32_t hours = ms / 3600000;
    int32_t mins = (ms / 60000) % 60;
    int32_t secs = (ms / 1000) % 60;

    if (hours > 99) return std::string("");
    return std::format("{:02}:{:02}:{:02}", hours, mins, secs);
}

void GameTimer::update(bool playing) {
    if (playing && !clock.isRunning()) clock.start();
    else if (!playing) clock.stop();
}

void GameTimer::draw(sf::RenderWindow& window, sf::View& camera) {
    setTimerPosition(camera);
    time.setString(getElapsedFormatted());
    window.draw(time);
}