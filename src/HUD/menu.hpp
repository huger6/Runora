#pragma once

#include <iostream>
#include <string>
// SFML
#include <SFML/Graphics.hpp>
// HUD
#include "HUD/button.hpp"


class MainMenu {
    protected:
        sf::Vector2f centerPosition;
        sf::Color bgColor;
        sf::Texture bgTexture;
    public:
        Button startBtn;
        Button optionsBtn;
        Button quitBtn;
        sf::Sprite background;
        
        MainMenu(sf::RenderWindow& window);
        
        Button* getClickedButton(sf::RenderWindow& window);

        void update(sf::RenderWindow& window);
        void draw(sf::RenderWindow& window);
    private:
        sf::Text name;  
        sf::Text copyright;
};


class PauseMenu : public MainMenu {
    public:
        Button resumeBtn;
        Button returnToMenuBtn;

        PauseMenu(sf::RenderWindow& window);

        void update(sf::RenderWindow& window);
        void draw(sf::RenderWindow& window);
};


// class Menu {
//     public:
//     private:
//         sf::Color buttonColor;
//         sf::Color buttonTextColor;

// };