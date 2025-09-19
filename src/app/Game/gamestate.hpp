#pragma once

#include <iostream>
// SFML
#include <SFML/Graphics.hpp>
// Menu
#include <HUD/menu.hpp>

enum class GameState {
    MAIN_MENU,
    OPTIONS,
    PLAYING,
    PAUSE_MENU,
    EXIT
};

class GameStateManager {
    public:
        GameStateManager(sf::RenderWindow& window);

        GameState getCurrentState();
        void setCurrentState(GameState newState);
        bool isPlaying();

        void update(sf::RenderWindow& window);
    private:
        GameState currentState = GameState::MAIN_MENU;
        GameState lastDifferentState = GameState::MAIN_MENU;
        
        MainMenu mainMenu;
        PauseMenu pauseMenu;
}