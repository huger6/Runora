#include "gamestate.hpp"


// Public


GameStateManager::GameStateManager(sf::RenderWindow& window) : 
    mainMenu(window), 
    pauseMenu(window) {}

GameState GameStateManager::getCurrentState() const { return currentState; }

GameState GameStateManager::getLastDifferentState() const { return lastDifferentState; }

void GameStateManager::setCurrentState(GameState newState) {
    if (newState != GameState::OPTIONS) {
        lastDifferentState = currentState;
    }
    currentState = newState;
}

bool GameStateManager::isPlaying() {
    return currentState == GameState::PLAYING;
}

void GameStateManager::update(sf::RenderWindow& window) {
    switch(currentState) {
        case GameState::MAIN_MENU:
            mainMenu.update(window);
            
            // Redirect according to buttons pressed
            if (mainMenu.startBtn.isClicked(window)) setCurrentState(GameState::PLAYING);
            else if (mainMenu.optionsBtn.isClicked(window)) setCurrentState(GameState::OPTIONS);
            else if (mainMenu.quitBtn.isClicked(window)) setCurrentState(GameState::EXIT);
            break;
        case GameState::PAUSE_MENU:
            window.setView(window.getDefaultView());
            pauseMenu.update(window);

            // Redirect according to buttons pressed
            if (pauseMenu.resumeBtn.isClicked(window) || KeyClicked::isKeyClicked(sf::Keyboard::Key::Escape)) setCurrentState(GameState::PLAYING);
            else if (pauseMenu.optionsBtn.isClicked(window)) setCurrentState(GameState::OPTIONS);
            else if (pauseMenu.returnToMenuBtn.isClicked(window)) setCurrentState(GameState::MAIN_MENU);
            break;
        case GameState::OPTIONS:
            // Redirect to last menu
            if (KeyClicked::isKeyClicked(sf::Keyboard::Key::Escape)) setCurrentState(lastDifferentState);
            break;
        case GameState::EXIT:
            window.close();
            break;
        default: 
            break;
    }
}

void GameStateManager::draw(sf::RenderWindow& window) {
    if (getCurrentState() == GameState::MAIN_MENU) {
        mainMenu.draw(window);
    }
    else if (getCurrentState() == GameState::PAUSE_MENU) {
        pauseMenu.draw(window);
    }
}