#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <utils.hpp>

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
            return isHovered(window) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
        }

        void changeLabel(std::string& newLabel) {
            text.setString(newLabel);
            text.setOrigin(text.getLocalBounds().getCenter());
        }
};