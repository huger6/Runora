#pragma once

#include <iostream>
// SFML
#include <SFML/Graphics.hpp>
// Tile
#include "World/tile.hpp"
// Utils
#include "Utils/getters.hpp"

namespace PlayerConfigs {
    constexpr float PLAYER_SCALE_WIDTH = 2.0f;
    constexpr float PLAYER_SCALE_HEIGHT = 2.0f;
    constexpr float PLAYER_HITBOX_OFFSET_X = 1.0f * PLAYER_SCALE_WIDTH;
    constexpr float PLAYER_HITBOX_OFFSET_Y = 0.0f * PLAYER_SCALE_HEIGHT;
    constexpr float PLAYER_WIDTH = TileConfigs::TILE_SIZE * PLAYER_SCALE_WIDTH - PLAYER_HITBOX_OFFSET_X - 1.0f;
    constexpr float PLAYER_HEIGHT = (TileConfigs::TILE_SIZE * 2) * PLAYER_SCALE_HEIGHT - PLAYER_HITBOX_OFFSET_Y; 
    constexpr float PLAYER_INITIAL_POSITION_OFFSET = 106.0f;
};


class Player {
    public:
        sf::Vector2f playerPos;

        Player();
        Player(sf::Vector2f& playerPos);

        sf::Vector2f& getPosition();
        void checkMovement();

        void setHibbox();
        const sf::FloatRect& getHitbox() const;
        void drawHitbox(sf::RenderWindow& window);

        void update();
        void draw(sf::RenderWindow& window);
    private:
        sf::Sprite player;
        sf::RectangleShape hitbox;
};