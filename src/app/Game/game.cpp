#include "game.hpp"
#include "gamestate.hpp"
#include <cmath>

// Public

Game::Game() : 
        registry(),
        window(sf::VideoMode({MapConfigs::SCREEN_SIZE_X, MapConfigs::SCREEN_SIZE_Y}), "Runora", sf::Style::Titlebar | sf::Style::Close),
        stateManager(window),
        player(),
        terrain(registry),
        camera(player.getPosition(), {MapConfigs::SCREEN_SIZE_X, MapConfigs::SCREEN_SIZE_Y}),
        orbRegistry(),
        orbGenerator(terrain.getSpawningOrbsLength(), orbRegistry),
        hud(),
        isRunning(true),
        firstGame(true) {
            window.setVerticalSyncEnabled(false);
            window.setFramerateLimit(60);

            registry.loadFromJSON(getTilesetJSONPath());
            orbRegistry.loadFromJSON(getOrbsetJSONPath());
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
    // Calculate delta time for physics
    float deltaTime = deltaClock.restart().asSeconds();
    // Clamp deltaTime to prevent physics issues on lag spikes
    deltaTime = std::min(deltaTime, 0.05f);

    // Check if it's the first game
    if (stateManager.getCurrentState() == GameState::MAIN_MENU) firstGame = true;
    
    // Playing state 
    if (stateManager.isPlaying()) {
        if (firstGame) {
            hud.resetTimer();
            firstGame = false;
        }
        player.update(deltaTime);
        // Snap camera to nearest pixel to prevent sub-pixel tile rendering artifacts
        sf::Vector2f camCenter = player.getPosition();
        camCenter.x = std::round(camCenter.x);
        camCenter.y = std::round(camCenter.y);
        camera.setCenter(camCenter);
        window.setView(camera);
        // Update terrain
        terrain.update(player.getPosition());
        // Update orbs
        orbGenerator.update(player.getPosition(), player.getHitbox(), camera);
        // Update HUD
        hud.update(orbGenerator.getNumberOfOrbs(), true); // Needs checking to start timer
        if (KeyClicked::isKeyClicked(sf::Keyboard::Key::Escape)) stateManager.setCurrentState(GameState::PAUSE_MENU);
    }
    else {
        stateManager.update(window);
        hud.update(0, false); // Needs checking to start timer
    }
    // std::cout << "Player pos: (" << player.getPosition().x << "," << player.getPosition().y << ")\n";
}

void Game::render() {
    window.clear();
    if (stateManager.isPlaying()) {
        terrain.draw(window);
        orbGenerator.draw(window);
        player.draw(window);
        hud.draw(window, camera);
    }
    else {
        stateManager.draw(window);
    }
    window.display();
}