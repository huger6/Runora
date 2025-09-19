#pragma once

#include <iostream>
#include <string>
// SFML
#include <SFML/Graphics.hpp>
// HUD
#include <HUD/button.hpp>


class MainMenu {
    public:
        Button startBtn;
        Button optionsBtn;
        Button quitBtn;
        sf::Sprite background;
        
        MainMenu(sf::RenderWindow& window);
        
        Button* getClickedButton(sf::RenderWindow& window);

        void draw(sf::RenderWindow& window);
        void update(sf::RenderWindow& window);
    private:
        sf::Text name;  
        sf::Text copyright;
    protected:
        sf::Color bgColor;
        sf::Vector2f centerPosition;
        sf::Texture bgTexture;
};


class PauseMenu : public MainMenu {
    public:
        Button resumeBtn;
        Button returnToMenuBtn;

        PauseMenu(sf::RenderWindow& window);

        void update(sf::RenderWindow& window);
        void draw(sf::RenderWindow& window);
};