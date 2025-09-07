#include <SFML/Graphics.hpp>
#include <iostream>
#include <menu.hpp>
#include <time.hpp>
#include <button.hpp>
#include <tile.hpp>
#include <getters.hpp>
#include <grid.hpp>
#include <map.hpp>

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
    sf::Vector2f playerPos(MapConfigs::STARTING_POSITION_X, MapConfigs::STARTING_POSITION_Y);
    
    // Camera 
    sf::View camera({MapConfigs::STARTING_POSITION_X, MapConfigs::STARTING_POSITION_Y}, {MapConfigs::SCREEN_SIZE_X, MapConfigs::SCREEN_SIZE_Y});
    
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
                // Game logic 
                if (firstTimePlaying) {
                    timer.reset();
                    firstTimePlaying = false;
                }
                if (lastState != state) setMouseAsArrow(window);
                window.setView(camera);

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) playerPos.y -= 16;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) playerPos.y += 16;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) playerPos.x -= 16;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) playerPos.x += 16;
                
                // Ensure X coordinates never go negative
                playerPos.x = std::max(0.0f, playerPos.x);
                // Vertical limits to allow only jump and running
                
                //playerPos.y = std::min(MapConfigs::GROUND_Y, std::max(playerPos.y, MapConfigs::JUMPING_MAX_Y));

                playerPos.y = std::max(0.0f, playerPos.y); // Debug
                // Inicialize timer
                timer.resume();


                // Output player position for debugging
                std::cout << "Player Position: (" << playerPos.x << ", " << playerPos.y << ")" << std::endl;

                // Update the grid chunks based on player position
                camera.setCenter(playerPos);
                window.setView(camera);
                grid.updateChunks(playerPos);


                grid.draw(window);
                timer.draw(window);

                // States checking
                lastState = state;
                if (KeyClicked::isKeyClicked(sf::Keyboard::Key::Escape)) {
                    lastDifferentState = state;
                    state = GameState::PAUSE_MENU;
                    timer.pause();
                }
                break;

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