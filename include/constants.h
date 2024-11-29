// -*- C++ -*-
/// @file Contains global constants for configuration

#pragma once

#include <cstdint>
#include <filesystem>
#include <utl_SDLInterface.hpp>

namespace constants {

inline const std::string tetrisTitle{"Tetris"};
inline const std::string tetrisVersion{"0.0.1"};
inline const std::string tetrisIdentifier{"com.tetris.tetris"};
inline constexpr int screenWidth{1024};
inline constexpr int screenHeight{768};
inline constexpr unsigned sdlFlags{SDL_INIT_VIDEO};
inline constexpr unsigned windowFlags{0};

inline const std::filesystem::path titleFontPath{"data/Silkscreen-Bold.ttf"};
inline const std::filesystem::path instructionsFontPath{
    "data/Silkscreen-Regular.ttf"};
inline constexpr int32_t titleFontSize{128};
inline constexpr int32_t instructionsFontSize{48};

inline constexpr int cellWidth{32};
inline constexpr int cellHeight{32};

inline constexpr unsigned shapeWidth{4};
inline constexpr unsigned shapeHeight{4};

inline const std::array<int, shapeWidth * shapeHeight> I_tetromino{
    0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};

inline const std::array<int, shapeWidth * shapeHeight> O_tetromino{
    0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0};

inline const std::array<int, shapeWidth * shapeHeight> T_tetromino{
    0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0};

inline const std::array<int, shapeWidth * shapeHeight> J_tetromino{
    0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0};

inline const std::array<int, shapeWidth * shapeHeight> L_tetromino{
    1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0};

inline const std::array<int, shapeWidth * shapeHeight> S_tetromino{
    0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

inline const std::array<int, shapeWidth * shapeHeight> Z_tetromino{
    1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

inline constexpr unsigned gridWidth{10};
inline constexpr unsigned gridHeight{20};
inline constexpr int gridWallThickness{16};
inline constexpr double gridPosX{screenWidth / 2 - (cellWidth * (gridWidth / 2))
                                 - gridWallThickness};
inline constexpr double gridPosY{
    screenHeight / 2 - (cellHeight * (gridHeight / 2)) - gridWallThickness};
inline constexpr unsigned gridWalls{4};

}  // namespace constants
