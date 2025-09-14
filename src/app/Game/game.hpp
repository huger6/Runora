#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <World/map.hpp>

class Game {
    private:
        sf::RenderWindow window;
        

        void update() {

        }

        void render() {

        }
    public:
        Game() : window(sf::VideoMode({MapConfigs::SCREEN_SIZE_X, MapConfigs::SCREEN_SIZE_Y}), "Runora", sf::Style::Titlebar | sf::Style::Close) {
            window.setVerticalSyncEnabled(false);
            window.setFramerateLimit(60);
        }

        void run() {

        }


}