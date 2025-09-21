#pragma once

#include <iostream>
// SFML
#include <SFML/Graphics.hpp>
// Utils
#include "Utils/utils.hpp"

class KeyClicked {
    public:
        KeyClicked();

        static bool isKeyClicked(sf::Keyboard::Key key) {
            bool pressed = sf::Keyboard::isKeyPressed(key);

            if (pressed && !wasPressed[static_cast<size_t>(key)]) {
                wasPressed[static_cast<size_t>(key)] = true;
                return true;
            }
            if (!pressed) {
                wasPressed[static_cast<size_t>(key)] = false;
            }

            return false;
        }
    private:
        static inline std::array<bool, sf::Keyboard::KeyCount> wasPressed{};
};

class MouseClicked {
    public:
        MouseClicked();

        static bool isBtnClicked(sf::Mouse::Button btn) {
            bool pressed = sf::Mouse::isButtonPressed(btn);

            if (pressed && !wasPressed[static_cast<size_t>(btn)]) {
                wasPressed[static_cast<size_t>(btn)] = true;
                return true;
            }
            if (!pressed) {
                wasPressed[static_cast<size_t>(btn)] = false;
            }

            return false;
        }
    private:
        static inline std::array<bool, sf::Mouse::ButtonCount> wasPressed{};
};


class Button {
    // I know this code's trash, but it works
    public:
        Button();
        
        Button(const sf::Vector2f& size,
            unsigned int textSize, 
            const sf::Vector2f& position, 
            const sf::Font& font, 
            const std::string& label, 
            const sf::Color& bgColor, 
            const sf::Color& textColor, 
            const sf::Color& hoverColor);
        
        void changeLabel(std::string& newLabel);

        bool isHovered(const sf::RenderWindow& window);
        bool isClicked(const sf::RenderWindow& window);
        void update(sf::RenderWindow& window);
        void draw(sf::RenderWindow& window);
    private:
        sf::RectangleShape shape;
        sf::Color bgColor;  
        sf::Color textColor;
        sf::Color hoverColor;
        sf::Text text;
};