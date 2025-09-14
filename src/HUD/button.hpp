#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Utils/utils.hpp>

class KeyClicked {
    private:
        static inline std::array<bool, sf::Keyboard::KeyCount> wasPressed{};
    public:
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
};

class MouseClicked {
    private:
        static inline std::array<bool, sf::Mouse::ButtonCount> wasPressed{};
    public:
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
};

class Button {
    private:
        sf::RectangleShape shape;
        sf::Color bgColor;  
        sf::Color textColor;
        sf::Color hoverColor;
        sf::Text text;
    public:
        Button() = default;
        
        Button(const sf::Vector2f& size, unsigned int textSize, const sf::Vector2f& position, const sf::Font& font, const std::string& label, const sf::Color& bgColor, const sf::Color& textColor, const sf::Color& hoverColor)
        : bgColor(bgColor), textColor(textColor), hoverColor(hoverColor), text(font) {
            shape.setSize(size);
            shape.setOrigin(shape.getLocalBounds().getCenter());
            shape.setPosition(position);
            shape.setFillColor(bgColor);

            text.setString(label);
            text.setCharacterSize(textSize);
            text.setFillColor(textColor);
            text.setOrigin(text.getLocalBounds().getCenter());
            text.setPosition(position);
        }

        void draw(sf::RenderWindow& window) {
            window.draw(shape);
            window.draw(text);
        }

        bool isHovered(const sf::RenderWindow& window) {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            sf::Vector2f posInFloat = {static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)};
            return shape.getGlobalBounds().contains(posInFloat);
        }

        void update(sf::RenderWindow& window) {
            if (isHovered(window)) {
                shape.setFillColor(hoverColor);
            }
            else {
                shape.setFillColor(bgColor);
            }
        }

        bool isClicked(const sf::RenderWindow& window) {
            return isHovered(window) && MouseClicked::isBtnClicked(sf::Mouse::Button::Left);
        }

        void changeLabel(std::string& newLabel) {
            text.setString(newLabel);
            text.setOrigin(text.getLocalBounds().getCenter());
        }
};