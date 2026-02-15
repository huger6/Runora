#pragma once

#include <iostream>
// SFML
#include <SFML/Graphics.hpp>
// World
#include "World/tile.hpp"
#include "World/terrainGenerator.hpp"
#include "World/map.hpp"
#include "World/orbGenerator.hpp"
// Player
#include "Player/player.hpp"
// HUD
#include "HUD/hud.hpp"
// Gamestate
#include "gamestate.hpp"


class Game {
    public:
        Game();

        void run();
    private:
        TileRegistry registry;
        sf::RenderWindow window;
        GameStateManager stateManager;
        Player player;
        TerrainGenerator terrain;
        sf::View camera;
        OrbRegistry orbRegistry;
        OrbGenerator orbGenerator;
        HUD hud;
        bool isRunning;
        bool firstGame;
        sf::Clock deltaClock;  // Clock for deltaTime calculation
        
        void processEvents();
        void update();
        void render();
};