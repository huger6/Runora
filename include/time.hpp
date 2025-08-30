#pragma once
#include <SFML/System.hpp>
#include <iostream>
#include <string>

class GameTimer {
    private:
        sf::Clock clock;

        sf::Time getElapsed() const { return clock.getElapsedTime(); }
    public:
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
};
