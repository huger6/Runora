#include "game.hpp"
#include "gamestate.hpp"

// Public

Game::Game() : 
        registry(),
        window(sf::VideoMode({MapConfigs::SCREEN_SIZE_X, MapConfigs::SCREEN_SIZE_Y}), "Runora", sf::Style::Titlebar | sf::Style::Close),
        stateManager(window),
        player(),
        terrain(registry),
        camera(player.getPosition(), {MapConfigs::SCREEN_SIZE_X, MapConfigs::SCREEN_SIZE_Y}),
        orbGenerator(terrain.getSpawningOrbsLength()),
        hud(),
        isRunning(true),
        firstGame(true) {
    window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(60);

    registry.loadFromJSON(getTilesetJSONPath());
}

void Game::run() {
    while (window.isOpen() && isRunning) {
        processEvents();
        update();
        render();
    }
}


// Private


void Game::processEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            isRunning = false;
        }
    }
}

void Game::update() {
    // Check if it's the first game
    if (stateManager.getCurrentState() == GameState::MAIN_MENU) firstGame = true;
    
    // Playing state 
    if (stateManager.isPlaying()) {
        if (firstGame) {
            hud.resetTimer();
            firstGame = false;
        }
        player.update();
        // Update camera based on player's position
        camera.setCenter(player.getPosition());
        window.setView(camera);
        terrain.update(player.getPosition());
        // ERROR HERE!
        orbGenerator.update(terrain.getSpawningOrbsLimitsX(player.getPosition()), player.getHitbox(), camera);
        hud.update(orbGenerator.getNumberOfOrbs(), true); // Needs checking to start timer
        if (KeyClicked::isKeyClicked(sf::Keyboard::Key::Escape)) stateManager.setCurrentState(GameState::PAUSE_MENU);
    }
    else {
        stateManager.update(window);
        hud.update(0, false); // Needs checking to start timer
    }
}

void Game::render() {
    window.clear();
    if (stateManager.isPlaying()) {
        terrain.draw(window);
        player.draw(window);
        orbGenerator.draw(window);
        hud.draw(window, camera);
    }
    else {
        stateManager.draw(window);
    }
    window.display();
}