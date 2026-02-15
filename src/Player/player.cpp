#include "player.hpp"
// MapConfigs
#include "World/map.hpp"
#include <cmath>
#include <algorithm>


// Public


Player::Player() : 
    playerPos({MapConfigs::STARTING_POSITION_X, MapConfigs::STARTING_POSITION_Y}),
    player(getPlayerTexture()),
    velocity({0.0f, 0.0f}),
    acceleration({0.0f, 0.0f}),
    isGrounded(true),
    jumpsRemaining(PlayerConfigs::MAX_JUMPS),
    jumpKeyWasPressed(false),
    facing(FacingDirection::East),
    animFrame(0),
    animTimer(0.0f),
    isDoubleJumping(false),
    wasRunningBeforeJump(false) {
        player.setOrigin(player.getGlobalBounds().getCenter());
        player.setScale({PlayerConfigs::PLAYER_SCALE_WIDTH, PlayerConfigs::PLAYER_SCALE_HEIGHT});
        setHitbox(); // Debug
}

Player::Player(sf::Vector2f& playerPos) : 
    playerPos(playerPos),
    player(getPlayerTexture()),
    velocity({0.0f, 0.0f}),
    acceleration({0.0f, 0.0f}),
    isGrounded(true),
    jumpsRemaining(PlayerConfigs::MAX_JUMPS),
    jumpKeyWasPressed(false),
    facing(FacingDirection::East),
    animFrame(0),
    animTimer(0.0f),
    isDoubleJumping(false),
    wasRunningBeforeJump(false) {
        player.setOrigin(player.getGlobalBounds().getCenter());
        player.setScale({PlayerConfigs::PLAYER_SCALE_WIDTH, PlayerConfigs::PLAYER_SCALE_HEIGHT});
        this->setHitbox(); // Debug
}

sf::Vector2f& Player::getPosition() {
    return playerPos;
}

void Player::handleInput(float /* deltaTime */) {
    // Reset horizontal acceleration
    acceleration.x = 0.0f;

    // Horizontal movement: A (left) and D (right)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        acceleration.x -= PlayerConfigs::HORIZONTAL_ACCELERATION;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        acceleration.x += PlayerConfigs::HORIZONTAL_ACCELERATION;
    }

    // Jumping with Space - detect new key press for double jump
    bool jumpKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
    if (jumpKeyPressed && !jumpKeyWasPressed) {
        jump();
    }
    jumpKeyWasPressed = jumpKeyPressed;
}

void Player::applyPhysics(float deltaTime) {
    // Compute all accelerations before integration
    applyGravity();

    // Apply friction as an opposing acceleration when no horizontal input
    float totalAccelX = acceleration.x;
    bool applyingFriction = (acceleration.x == 0.0f && velocity.x != 0.0f);
    if (applyingFriction) {
        totalAccelX = (velocity.x > 0.0f)
            ? -PlayerConfigs::HORIZONTAL_DECELERATION
            :  PlayerConfigs::HORIZONTAL_DECELERATION;
    }

    // Velocity Verlet integration: x(t+dt) = x(t) + v(t)*dt + 0.5*a*dt²
    playerPos.x += velocity.x * deltaTime + 0.5f * totalAccelX * deltaTime * deltaTime;
    playerPos.y += velocity.y * deltaTime + 0.5f * acceleration.y * deltaTime * deltaTime;

    // Update velocity: v(t+dt) = v(t) + a*dt
    float prevVelX = velocity.x;
    velocity.x += totalAccelX * deltaTime;
    velocity.y += acceleration.y * deltaTime;

    // Prevent friction from reversing horizontal direction
    if (applyingFriction && ((prevVelX > 0.0f && velocity.x < 0.0f) || (prevVelX < 0.0f && velocity.x > 0.0f))) {
        velocity.x = 0.0f;
    }

    // Clamp velocities to maximum values
    velocity.x = std::clamp(velocity.x, -PlayerConfigs::MAX_HORIZONTAL_VELOCITY, PlayerConfigs::MAX_HORIZONTAL_VELOCITY);
    velocity.y = std::clamp(velocity.y, -PlayerConfigs::MAX_VERTICAL_VELOCITY, PlayerConfigs::MAX_VERTICAL_VELOCITY);

    // Check ground collision and boundaries
    checkGroundCollision();

    // Ensure X coordinates never go negative
    if (playerPos.x < 0.0f) {
        playerPos.x = 0.0f;
        velocity.x = 0.0f;
    }
}

void Player::applyGravity() {
    // Constant gravitational acceleration (Newton's law of gravitation simplified)
    // F = mg, and since a = F/m, a = g (mass cancels out)
    acceleration.y = PlayerConfigs::GRAVITY;
}

void Player::checkGroundCollision() {
    // Ground is at PLAYER_MIN_Y (from the original constraints)
    if (playerPos.y >= MapConfigs::PLAYER_MIN_Y) {
        playerPos.y = MapConfigs::PLAYER_MIN_Y;
        velocity.y = 0.0f;
        isGrounded = true;
        jumpsRemaining = PlayerConfigs::MAX_JUMPS;  // Reset jumps when grounded
        isDoubleJumping = false;                    // Reset double jump animation
        wasRunningBeforeJump = false;               // Reset running-before-jump flag
    } else {
        isGrounded = false;
    }
}

void Player::jump() {
    if (jumpsRemaining > 0) {
        if (isGrounded) {
            // First jump - freeze the current animation frame
            wasRunningBeforeJump = std::abs(velocity.x) > 1.0f;
            velocity.y = PlayerConfigs::JUMP_IMPULSE;
            isGrounded = false;
            isDoubleJumping = false;
        } else {
            // Double jump (air jump) - apply slightly weaker impulse
            velocity.y = PlayerConfigs::DOUBLE_JUMP_IMPULSE;
            isDoubleJumping = true;
            // Immediately swap animation frame on double jump
            animFrame = 1 - animFrame;
            animTimer = 0.0f;
        }
        jumpsRemaining--;
    }
}

void Player::setHitbox() {
    hitbox.setSize({PlayerConfigs::PLAYER_WIDTH, PlayerConfigs::PLAYER_HEIGHT});
    hitbox.setOrigin({hitbox.getLocalBounds().size.x / 2.0f - PlayerConfigs::PLAYER_HITBOX_OFFSET_X, hitbox.getLocalBounds().size.y / 2.0f + PlayerConfigs::PLAYER_HITBOX_OFFSET_Y});
    
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineThickness(1.0f);
    hitbox.setOutlineColor(sf::Color::Red);
}

const sf::FloatRect Player::getHitbox() const {
    return hitbox.getGlobalBounds();
}

void Player::drawHitbox(sf::RenderWindow& window) {
    hitbox.setPosition({playerPos.x, playerPos.y + PlayerConfigs::PLAYER_INITIAL_POSITION_OFFSET});
    window.draw(hitbox);
}

void Player::update(float deltaTime) {
    handleInput(deltaTime);
    applyPhysics(deltaTime);
    updateAnimation(deltaTime);
    hitbox.setPosition({playerPos.x, playerPos.y + PlayerConfigs::PLAYER_INITIAL_POSITION_OFFSET});
}

void Player::draw(sf::RenderWindow& window) {
    applyTexture();
    applyFacing();
    player.setPosition({playerPos.x, playerPos.y + PlayerConfigs::PLAYER_INITIAL_POSITION_OFFSET});
    window.draw(player);
    // drawHitbox(window);
}

bool Player::getIsGrounded() const {
    return isGrounded;
}

void Player::reset() {
    playerPos = {MapConfigs::STARTING_POSITION_X, MapConfigs::STARTING_POSITION_Y};
    velocity = {0.0f, 0.0f};
    acceleration = {0.0f, 0.0f};
    isGrounded = true;
    jumpsRemaining = PlayerConfigs::MAX_JUMPS;
    jumpKeyWasPressed = false;
    facing = FacingDirection::East;
    animFrame = 0;
    animTimer = 0.0f;
    isDoubleJumping = false;
    wasRunningBeforeJump = false;
}


// Animation


void Player::updateAnimation(float deltaTime) {
    // Update facing direction based on horizontal velocity
    if (velocity.x > 0.0f) {
        facing = FacingDirection::East;
    } else if (velocity.x < 0.0f) {
        facing = FacingDirection::West;
    }
    // When velocity is 0, keep the last facing direction

    bool isRunning = isGrounded && std::abs(velocity.x) > 1.0f;

    if (isRunning) {
        // On ground and moving: animate normally
        animTimer += deltaTime;
        if (animTimer >= PlayerConfigs::ANIMATION_FRAME_DURATION) {
            animTimer -= PlayerConfigs::ANIMATION_FRAME_DURATION;
            animFrame = 1 - animFrame;
        }
    } else if (isDoubleJumping) {
        // Double jump: frame was swapped once in jump(), freeze it until landing
    } else if (!isGrounded) {
        // Single jump or double jump without moving: freeze current frame
        // (don't change animFrame or animTimer)
    } else {
        // Grounded and idle: reset to idle
        animFrame = 0;
        animTimer = 0.0f;
    }
}

void Player::applyTexture() {
    bool isRunning = isGrounded && std::abs(velocity.x) > 1.0f;
    bool useRunningTex = isRunning
                      || isDoubleJumping
                      || (!isGrounded && wasRunningBeforeJump);

    if (useRunningTex) {
        if (animFrame == 0) {
            player.setTexture(getPlayerRunTexture1());
        } else {
            player.setTexture(getPlayerRunTexture2());
        }
    } else {
        player.setTexture(getPlayerTexture());
    }
    sf::FloatRect localBounds = player.getLocalBounds();
    player.setOrigin({localBounds.size.x / 2.0f, localBounds.size.y / 2.0f});
}

void Player::applyFacing() {
    // Textures are drawn facing East; flip X scale for West
    float scaleX = PlayerConfigs::PLAYER_SCALE_WIDTH;
    if (facing == FacingDirection::West) {
        scaleX = -scaleX;
    }
    player.setScale({scaleX, PlayerConfigs::PLAYER_SCALE_HEIGHT});
}