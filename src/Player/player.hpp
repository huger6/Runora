#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <World/map.hpp>
#include <Utils/getters.hpp>
#include <World/tile.hpp>

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
    private:
        sf::Sprite player;
        sf::RectangleShape hitbox;
    public:
        sf::Vector2f playerPos;
        Player() : player(getPlayerTexture()) {
            playerPos.x = MapConfigs::STARTING_POSITION_X;
            playerPos.y = MapConfigs::STARTING_POSITION_Y;

            player.setOrigin(player.getGlobalBounds().getCenter());
            player.setScale({PlayerConfigs::PLAYER_SCALE_WIDTH, PlayerConfigs::PLAYER_SCALE_HEIGHT});
            this->setHibbox(); // Debug
        }

        Player(sf::Vector2f& playerPos) : player(getPlayerTexture()), playerPos(playerPos) {
            player.setOrigin(player.getGlobalBounds().getCenter());
            player.setScale({PlayerConfigs::PLAYER_SCALE_WIDTH, PlayerConfigs::PLAYER_SCALE_HEIGHT});
            this->setHibbox(); // Debug
        }

        sf::Vector2f& getPosition() {
            return playerPos;
        }

        void checkMovement() {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) playerPos.y -= 16;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) playerPos.y += 16;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) playerPos.x -= 16;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) playerPos.x += 16;

            // Ensure X coordinates never go negative
            playerPos.x = std::max(0.0f, playerPos.x);
            // Vertical limits to allow only jump and running
            playerPos.y = std::min(MapConfigs::GROUND_Y, std::max(playerPos.y, MapConfigs::JUMPING_MAX_Y));
            // playerPos.y = std::max(0.0f, playerPos.y); // Debug
            // Output player position for debugging
            // std::cout << "Player Position: (" << playerPos.x << ", " << playerPos.y << ")" << std::endl;
        }

        void setHibbox() {
            hitbox.setSize({PlayerConfigs::PLAYER_WIDTH, PlayerConfigs::PLAYER_HEIGHT});
            hitbox.setOrigin({hitbox.getLocalBounds().size.x / 2.0f - PlayerConfigs::PLAYER_HITBOX_OFFSET_X, hitbox.getLocalBounds().size.y / 2.0f + PlayerConfigs::PLAYER_HITBOX_OFFSET_Y});
           
            hitbox.setFillColor(sf::Color::Transparent);
            hitbox.setOutlineThickness(1.0f);
            hitbox.setOutlineColor(sf::Color::Red);
        }

        const sf::RectangleShape& getHitbox() const {
            return hitbox;
        }

        void drawHitbox(sf::RenderWindow& window) {
            hitbox.setPosition({playerPos.x, playerPos.y + PlayerConfigs::PLAYER_INITIAL_POSITION_OFFSET});
            window.draw(hitbox);
        }

        void draw(sf::RenderWindow& window) {
            player.setPosition({playerPos.x, playerPos.y + PlayerConfigs::PLAYER_INITIAL_POSITION_OFFSET});
            window.draw(player);
            drawHitbox(window);
        }
};