#pragma once

#include <iostream>
#include "World/tile.hpp"

namespace MapConfigs {
    constexpr uint16_t TILES_AIR_VISIBLE = 30 * TileConfigs::TILE_SIZE;
    constexpr uint16_t TILES_BRICK_VISIBLE = 15 * TileConfigs::TILE_SIZE;
    constexpr uint16_t CHUNK_Y_AIR_VISIBLE = 6; // less than
    constexpr uint16_t CHUNK_Y_CLOUDS_VISIBLE = 4; // less than
    constexpr float STARTING_POSITION_X = 5400.0f;
    constexpr float STARTING_POSITION_Y = 1800.0f;
    constexpr uint16_t SCREEN_SIZE_X = 1280;
    constexpr uint16_t SCREEN_SIZE_Y = 720;
    constexpr float GROUND_Y = 1936.f;
    constexpr float JUMPING_MAX_Y = GROUND_Y - 12 * TileConfigs::TILE_SIZE;
    constexpr float PLAYER_MIN_Y = 1800.0f;
    constexpr float PLAYER_MAX_Y = PLAYER_MIN_Y - 6 * TileConfigs::TILE_SIZE;
    constexpr float MAX_Y_ORBS = GROUND_Y - TileConfigs::TILE_SIZE;
    constexpr float MIN_Y_ORBS = MAX_Y_ORBS - 6 * TileConfigs::TILE_SIZE;
};