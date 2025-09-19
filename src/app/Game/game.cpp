#include <game.hpp>
#include <gamestate.hpp>

// Public

Game::Game() : 
            registry(),
            window(sf::VideoMode({MapConfigs::SCREEN_SIZE_X, MapConfigs::SCREEN_SIZE_Y}), "Runora", sf::Style::Titlebar | sf::Style::Close),
            stateManager(window),
            player(),
            terrain(registry),
            camera(player.getPosition(), {MapConfigs::SCREEN_SIZE_X, MapConfigs::SCREEN_SIZE_Y}),
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
    window.clear();
    stateManager.update(window);
    if (stateManager.isPlaying()) {
        player.update();
        // Get Orbs acquired and pass to hud.update()
        hud.update(1);
        
    }
}

void Game::render() {

}