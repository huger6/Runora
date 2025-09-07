#pragma once

#include <iostream>
#include <tile.hpp>

namespace MapConfigs {
    constexpr uint16_t TILES_AIR_VISIBLE = 30 * TileConfigs::TILE_SIZE;
    constexpr uint16_t TILES_BRICK_VISIBLE = 15 * TileConfigs::TILE_SIZE;
    constexpr uint16_t CHUNK_Y_AIR_VISIBLE = 6; // less than
    constexpr uint16_t CHUNK_Y_CLOUDS_VISIBLE = 4; // less than
    constexpr float STARTING_POSITION_X = 5400.0f;
    constexpr float STARTING_POSITION_Y = 360.0f * 5;
    constexpr uint16_t SCREEN_SIZE_X = 1280;
    constexpr uint16_t SCREEN_SIZE_Y = 720;
    constexpr float GROUND_Y = STARTING_POSITION_Y + TILES_AIR_VISIBLE;
    constexpr float JUMPING_MAX_Y = GROUND_Y - 4 * TileConfigs::TILE_SIZE;
};