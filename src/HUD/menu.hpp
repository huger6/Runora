#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>
// SFML
#include <SFML/Graphics.hpp>
// HUD
#include "HUD/button.hpp"


// Estrutura para configuração de botões
struct ButtonConfig {
    std::string label;
    float yOffset;  // Offset vertical a partir do centro
};


// Classe base para todos os menus
class Menu {
    public:
        Menu(sf::RenderWindow& window, const std::string& backgroundPath);
        virtual ~Menu() = default;

        // Retorna o índice do botão clicado, ou -1 se nenhum
        int getClickedButtonIndex(sf::RenderWindow& window);
        
        // Retorna o botão clicado por label, ou nullptr se nenhum
        Button* getClickedButton(sf::RenderWindow& window);

        virtual void update(sf::RenderWindow& window);
        virtual void draw(sf::RenderWindow& window);

    protected:
        sf::Vector2f centerPosition;
        sf::Color themeColor;
        sf::Texture bgTexture;
        sf::Sprite background;
        std::vector<Button> buttons;

        // Método auxiliar para adicionar botões
        void addButton(const std::string& label, float yOffset);
        
        // Configuração padrão de botões
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

        // Acesso aos botões por nome para compatibilidade
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

        // Acesso aos botões por nome para compatibilidade
        Button& getResumeButton() { return buttons[0]; }
        Button& getOptionsButton() { return buttons[1]; }
        Button& getReturnToMenuButton() { return buttons[2]; }
};