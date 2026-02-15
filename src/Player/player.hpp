#pragma once

#include <iostream>
// SFML
#include <SFML/Graphics.hpp>
// Tile
#include "World/tile.hpp"
// Utils
#include "Utils/getters.hpp"

enum class FacingDirection { East, West };

namespace PlayerConfigs {
    constexpr float ANIMATION_FRAME_DURATION = 0.15f;       // Seconds per animation frame
    constexpr float PLAYER_SCALE_WIDTH = 2.0f;
    constexpr float PLAYER_SCALE_HEIGHT = 2.0f;
    constexpr float PLAYER_HITBOX_OFFSET_X = 1.0f * PLAYER_SCALE_WIDTH;
    constexpr float PLAYER_HITBOX_OFFSET_Y = 0.0f * PLAYER_SCALE_HEIGHT;
    constexpr float PLAYER_WIDTH = TileConfigs::TILE_SIZE * PLAYER_SCALE_WIDTH - PLAYER_HITBOX_OFFSET_X - 1.0f;
    constexpr float PLAYER_HEIGHT = (TileConfigs::TILE_SIZE * 2) * PLAYER_SCALE_HEIGHT - PLAYER_HITBOX_OFFSET_Y; 
    constexpr float PLAYER_INITIAL_POSITION_OFFSET = 106.0f;

    // Physics constants (Newton's laws: F = ma, with constant gravity)
    constexpr float PLAYER_MASS = 70.0f;                    // Mass in kg
    constexpr float GRAVITY = 980.0f;                       // Gravity acceleration (pixels/s²)
    constexpr float HORIZONTAL_ACCELERATION = 2000.0f;      // Horizontal acceleration when moving
    constexpr float HORIZONTAL_DECELERATION = 1500.0f;      // Friction/deceleration when not moving
    constexpr float MAX_HORIZONTAL_VELOCITY = 400.0f;       // Maximum horizontal speed
    constexpr float MAX_VERTICAL_VELOCITY = 800.0f;         // Terminal velocity
    constexpr float JUMP_IMPULSE = -450.0f;                 // Initial jump velocity (negative = up)
    constexpr float DOUBLE_JUMP_IMPULSE = -400.0f;          // Double jump velocity
    constexpr int MAX_JUMPS = 2;                            // Maximum jumps (1 = normal, 2 = double jump)
};


class Player {
    public:
        sf::Vector2f playerPos;

        Player();
        Player(sf::Vector2f& playerPos);

        sf::Vector2f& getPosition();
        bool getIsGrounded() const;
        void handleInput(float deltaTime);
        void applyPhysics(float deltaTime);

        void setHitbox();
        const sf::FloatRect getHitbox() const;
        void drawHitbox(sf::RenderWindow& window);

        void update(float deltaTime);
        void draw(sf::RenderWindow& window);
        void reset();

    private:
        sf::Sprite player;
        sf::RectangleShape hitbox;

        // Physics state
        sf::Vector2f velocity;          // Current velocity (pixels/s)
        sf::Vector2f acceleration;      // Current acceleration (pixels/s^2)
        bool isGrounded;                // Whether player is on the ground
        int jumpsRemaining;             // Jumps left
        bool jumpKeyWasPressed;

        // Animation state
        FacingDirection facing;
        int animFrame;                  // Current animation frame (0 or 1)
        float animTimer;                // Timer for frame switching
        bool isDoubleJumping;
        bool wasRunningBeforeJump;

        // Physics helpers
        void applyGravity();
        void checkGroundCollision();
        void jump();

        // Animation helpers
        void updateAnimation(float deltaTime);
        void applyTexture();
        void applyFacing();
};