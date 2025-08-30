#include <iostream>
#include <SFML/Graphics.hpp>
#include <constants.hpp>
#include <menu.hpp>
#include <time.hpp>

void game(sf::RenderWindow& window) {
    GameTimer timer;
    timer.start();
    sf::Text time(getFont());
    time.setFillColor(sf::Color::White);
    time.setOrigin(time.getLocalBounds().getCenter());
    time.setPosition({30, 20});

    while(window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
            window.close();
        }
        window.clear();
        time.setString(timer.getElapsedFormatted());
        window.draw(time);
        window.display();
    }
}

void options(sf::RenderWindow& window) {

}