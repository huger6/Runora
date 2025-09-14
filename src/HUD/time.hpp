#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class GameTimer {
    private:
        sf::Clock clock;
        
        sf::Time getElapsed() const { return clock.getElapsedTime(); } // Get time elapsed until now

        void setTimerPosition(sf::View& view) {
            sf::Vector2f center = view.getCenter();
            sf::Vector2f size = view.getSize();
            float left = center.x - size.x / 2.0f;
            float top = center.y - size.y / 2.0f;
            time.setPosition({left + 30.0f, top + 20.0f});
        }
    protected:
        sf::Text time;
    public:
        explicit GameTimer(sf::View& view) : time(getFont()) {
            clock.start();

            time.setFillColor(sf::Color::Red);
            time.setOrigin(time.getLocalBounds().getCenter());

            setTimerPosition(view);
        }

        void start() {
            clock.restart();
        }

        void pause() {
            if (clock.isRunning()) clock.stop();
        }

        void resume() {
            if (!clock.isRunning()) clock.start();
        }

        void reset() {
            clock.reset();
        }

        inline std::string getElapsedFormatted() {
            sf::Time timeElapsed = getElapsed();
            int32_t ms = timeElapsed.asMilliseconds();
            int32_t hours = ms / 3600000;
            int32_t mins = (ms / 60000) % 60;
            int32_t secs = (ms / 1000) % 60;

            if (hours > 99) return std::string("");
            return std::format("{:02}:{:02}:{:02}", hours, mins, secs);
        }

        void draw(sf::RenderWindow& window, sf::View& view) {
            setTimerPosition(view);
            time.setString(getElapsedFormatted());
            window.draw(time);
        }
};
