#include <SFML/Graphics.hpp>
#include <iostream>
#include <menu.hpp>
#include <time.hpp>
#include <utils.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Runora", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(60);

    GameState state = GameState::MAIN_MENU;
    GameState previousState = GameState::MAIN_MENU; // Necessary for options menu callback
    bool firstTimePlaying = true;

    MainMenu mainMenu(window);
    PauseMenu pauseMenu(window);

    GameTimer timer;
    timer.start();
    sf::Text time(getFont());
    time.setFillColor(sf::Color::White);
    time.setOrigin(time.getLocalBounds().getCenter());
    time.setPosition({30, 20});

    while (window.isOpen()) {     
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
            window.close();
        }

        window.clear();

        switch(state) {
            case GameState::MAIN_MENU:
                mainMenu.update(window);
                mainMenu.draw(window);
                if (mainMenu.startBtn.isClicked(window)) {
                    state = GameState::PLAYING;
                }
                if (mainMenu.optionsBtn.isClicked(window)) {
                    previousState = state;
                    state = GameState::OPTIONS;
                }
                if (mainMenu.quitBtn.isClicked(window)) {
                    state = GameState::EXIT;
                }
                break;

            case GameState::PLAYING:
                // Game logic 
                if (firstTimePlaying) {
                    timer.reset();
                    firstTimePlaying = false;
                }
                
                timer.resume();
                time.setString(timer.getElapsedFormatted());
                window.draw(time);

                if (isEscClicked()) {
                    state = GameState::PAUSE_MENU;
                    timer.pause();
                }
                break;

            case GameState::PAUSE_MENU:
                pauseMenu.update(window);
                pauseMenu.draw(window);
                if (pauseMenu.resumeBtn.isClicked(window) || isEscClicked()) {
                    state = GameState::PLAYING;
                }
                if (pauseMenu.optionsBtn.isClicked(window)) {
                    previousState = state;
                    state = GameState::OPTIONS;
                }
                if (pauseMenu.returnToMenuBtn.isClicked(window)) {
                    state = GameState::MAIN_MENU;
                }
                break;

            case GameState::OPTIONS:
                if (isEscClicked()) {
                   state = previousState;
                }
                break;
            case GameState::EXIT:
                window.close();
                return 0;
            
        }
        window.display();
    }
}