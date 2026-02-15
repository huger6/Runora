#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>
// SFML
#include <SFML/Graphics.hpp>
// HUD
#include "HUD/button.hpp"


struct ButtonConfig {
    std::string label;
    float yOffset;
};


class Menu {
    public:
        Menu(sf::RenderWindow& window, const std::string& backgroundPath);
        virtual ~Menu() = default;

        // Retorna button clicked index or -1 if none
        int getClickedButtonIndex(sf::RenderWindow& window);
        
        // Returns button clicked or nullptr if none
        Button* getClickedButton(sf::RenderWindow& window);

        virtual void update(sf::RenderWindow& window);
        virtual void draw(sf::RenderWindow& window);

    protected:
        sf::Vector2f centerPosition;
        sf::Color themeColor;
        sf::Texture bgTexture;
        sf::Sprite background;
        std::vector<Button> buttons;

        void addButton(const std::string& label, float yOffset);
        
        // Default buttons config
        static constexpr float BUTTON_WIDTH = 200.0f;
        static constexpr float BUTTON_HEIGHT = 60.0f;
        static constexpr unsigned int BUTTON_TEXT_SIZE = 40;
        static constexpr float BUTTON_SPACING = 80.0f;

    private:
        void setupBackground(sf::RenderWindow& window, const std::string& backgroundPath);
};


class MainMenu : public Menu {
    public:
        MainMenu(sf::RenderWindow& window);

        void draw(sf::RenderWindow& window) override;

        Button& getStartButton() { return buttons[0]; }
        Button& getOptionsButton() { return buttons[1]; }
        Button& getQuitButton() { return buttons[2]; }

    private:
        sf::Text title;
        sf::Text copyright;

        void setupTitle(sf::RenderWindow& window);
        void setupCopyright(sf::RenderWindow& window);
};


class PauseMenu : public Menu {
    public:
        PauseMenu(sf::RenderWindow& window);

        Button& getResumeButton() { return buttons[0]; }
        Button& getOptionsButton() { return buttons[1]; }
        Button& getReturnToMenuButton() { return buttons[2]; }
};