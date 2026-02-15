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
        firstGame(true),
        playerDead(false),
        deathTimer(0.0f),
        deathText(getFont()) {
            window.setVerticalSyncEnabled(false);
            window.setFramerateLimit(60);

            registry.loadFromJSON(getTilesetJSONPath());
            orbRegistry.loadFromJSON(getOrbsetJSONPath());

            // Setup death overlay
            deathOverlay.setSize({static_cast<float>(MapConfigs::SCREEN_SIZE_X), static_cast<float>(MapConfigs::SCREEN_SIZE_Y)});
            deathOverlay.setFillColor(sf::Color(0, 0, 0, 180));

            deathText.setCharacterSize(50);
            deathText.setFillColor(sf::Color::Red);
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
        // Handle death screen countdown
        if (playerDead) {
            deathTimer -= deltaTime;
            if (deathTimer <= 0.0f) {
                resetGame();
            }
            return;
        }

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
        // Check if player stepped on a hazard (water/lava)
        checkHazardCollision();
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
        if (playerDead) {
            drawDeathScreen();
        }
    }
    else {
        stateManager.draw(window);
    }
    window.display();
}


// Death system


void Game::checkHazardCollision() {
    // Only check when player is on the ground (not jumping)
    if (!player.getIsGrounded()) return;

    // Calculate the tile coordinates under the player's feet
    float feetY = player.getPosition().y + PlayerConfigs::PLAYER_INITIAL_POSITION_OFFSET 
                  + PlayerConfigs::PLAYER_HEIGHT / 2.0f;
    float hitboxLeft = player.getPosition().x 
                       - (PlayerConfigs::PLAYER_WIDTH / 2.0f - PlayerConfigs::PLAYER_HITBOX_OFFSET_X);
    float hitboxRight = hitboxLeft + PlayerConfigs::PLAYER_WIDTH;

    int tileYFeet = static_cast<int>(floor(feetY / TileConfigs::TILE_SIZE));
    int tileXLeft = static_cast<int>(floor(hitboxLeft / TileConfigs::TILE_SIZE));
    int tileXRight = static_cast<int>(floor(hitboxRight / TileConfigs::TILE_SIZE));

    for (int tx = tileXLeft; tx <= tileXRight; tx++) {
        TileId tileId = terrain.getTileIdAtWorldPos(
            static_cast<float>(tx * TileConfigs::TILE_SIZE),
            static_cast<float>(tileYFeet * TileConfigs::TILE_SIZE));

        if (tileId == TileId::Water || tileId == TileId::Waves) {
            handleDeath("You drowned!");
            return;
        }
        if (tileId == TileId::Lava) {  
            handleDeath("You burned alive!");
            return;
        }
    }
}

void Game::handleDeath(const std::string& cause) {
    playerDead = true;
    deathTimer = DEATH_DISPLAY_TIME;
    deathText.setString(cause + "\nRestarting...");
    deathText.setOrigin(deathText.getLocalBounds().getCenter());
}

void Game::resetGame() {
    playerDead = false;
    deathTimer = 0.0f;
    player.reset();
    orbGenerator.reset();
    terrain.clear();
    firstGame = true;

    // Reset camera and regenerate terrain around the starting position
    camera.setCenter(player.getPosition());
    window.setView(camera);
    terrain.update(player.getPosition());
}

void Game::drawDeathScreen() {
    sf::Vector2f camCenter = camera.getCenter();
    sf::Vector2f camSize = camera.getSize();

    // Dark overlay
    deathOverlay.setPosition({camCenter.x - camSize.x / 2.0f, camCenter.y - camSize.y / 2.0f});
    window.draw(deathOverlay);

    // Death message centered on screen
    deathText.setPosition(camCenter);
    window.draw(deathText);
}