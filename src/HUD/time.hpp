#pragma once

#include <iostream>
#include <string>
// SFML
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>


class GameTimer {
    public:
        explicit GameTimer();

        void start();
        void pause();
        void resume();
        void reset();

        inline std::string getElapsedFormatted();
        sf::Time getElapsed() const;

        void update(bool playing);
        void draw(sf::RenderWindow& window, sf::View& camera);
    private:
        sf::Clock clock;
        sf::Text time;

        void setTimerPosition(sf::View& camera);
};
