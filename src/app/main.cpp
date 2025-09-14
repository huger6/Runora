#include <SFML/Graphics.hpp>
#include <iostream>
#include <HUD/menu.hpp>
#include <HUD/time.hpp>
#include <HUD/button.hpp>
#include <World/tile.hpp>
#include <Utils/getters.hpp>
#include <World/grid.hpp>
#include <World/map.hpp>
#include <Player/player.hpp>


int main() {
    sf::RenderWindow window(sf::VideoMode({MapConfigs::SCREEN_SIZE_X, MapConfigs::SCREEN_SIZE_Y}), "Runora", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(60);

    GameState state = GameState::MAIN_MENU;
    GameState lastDifferentState = GameState::MAIN_MENU; // Necessary for options menu callback
    GameState lastState = GameState::MAIN_MENU;

    bool firstTimePlaying = true;

    MainMenu mainMenu(window);
    PauseMenu pauseMenu(window);

    
    // Initialize registry
    TileRegistry registry;
    registry.loadFromJSON(getTilesetJSONPath());
    
    // Grid
    Grid grid(registry);
    
    // Player position
    Player player;
    
    // Camera 
    sf::View camera(player.getPosition(), {MapConfigs::SCREEN_SIZE_X, MapConfigs::SCREEN_SIZE_Y});
    
    GameTimer timer(camera);


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
                
                // States checking
                lastState = state;
                if (mainMenu.startBtn.isClicked(window)) {
                    lastDifferentState = state;
                    state = GameState::PLAYING;
                }
                else if (mainMenu.optionsBtn.isClicked(window)) {
                    lastDifferentState = state;
                    state = GameState::OPTIONS;
                }
                else if (mainMenu.quitBtn.isClicked(window)) {
                    lastDifferentState = state;
                    state = GameState::EXIT;
                }
                break;
            
            case GameState::PLAYING:
            {
                // Game logic 
                if (firstTimePlaying) {
                    timer.reset();
                    firstTimePlaying = false;
                }
                if (lastState != state) setMouseAsArrow(window);
                window.setView(camera);

                // Inicialize timer
                timer.resume();

                // Update player position
                player.checkMovement();
                
                // Update the grid chunks based on player position
                camera.setCenter(player.getPosition());
                window.setView(camera);
                grid.updateChunks(player.getPosition());


                grid.draw(window);
                timer.draw(window, camera);
                player.draw(window);

                // States checking
                lastState = state;
                if (KeyClicked::isKeyClicked(sf::Keyboard::Key::Escape)) {
                    lastDifferentState = state;
                    state = GameState::PAUSE_MENU;
                    timer.pause();
                }
                
                break;
            }
            case GameState::PAUSE_MENU:
                window.setView(window.getDefaultView());
                pauseMenu.update(window);
                pauseMenu.draw(window);

                // States checking
                lastState = state;
                if (pauseMenu.resumeBtn.isClicked(window) || KeyClicked::isKeyClicked(sf::Keyboard::Key::Escape)) {
                    lastDifferentState = state;
                    state = GameState::PLAYING;
                }
                else if (pauseMenu.optionsBtn.isClicked(window)) {
                    lastDifferentState = state;
                    state = GameState::OPTIONS;
                }
                else if (pauseMenu.returnToMenuBtn.isClicked(window)) {
                    lastDifferentState = state;
                    state = GameState::MAIN_MENU;
                }
                break;

            case GameState::OPTIONS:
                if (lastState != state) setMouseAsArrow(window); 
                // States checking
                lastState = state;
                if (KeyClicked::isKeyClicked(sf::Keyboard::Key::Escape)) {
                   state = lastDifferentState;
                }
                break;
            case GameState::EXIT:
                window.close();
                return 0;
            
        }
        window.display();
    }
}