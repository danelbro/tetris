// -*- C++ -*-
/// @file Contains global constants for configuration

#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <utl_SDLInterface.hpp>

namespace constants {
inline constexpr int shapeWidth{4};
inline constexpr int shapeHeight{4};
}  // namespace constants

struct TetrominoShape {
    std::array<int, constants::shapeWidth * constants::shapeHeight> shape;
};

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

inline const TetrominoShape I_tetromino{
    true,  true,  true,  true,  false, false, false, false,
    false, false, false, false, false, false, false, false};

inline const TetrominoShape O_tetromino{
    true,  true,  false, false, true,  true,  false, false,
    false, false, false, false, false, false, false, false};

inline const TetrominoShape T_tetromino{
    true,  true,  true,  false, false, true,  false, false,
    false, false, false, false, false, false, false, false};

inline const TetrominoShape J_tetromino{
    false, true, false, false, false, true,  false, false,
    true,  true, false, false, false, false, false, false};

inline const TetrominoShape L_tetromino{
    true, false, false, false, true,  false, false, false,
    true, true,  false, false, false, false, false, false};

inline const TetrominoShape S_tetromino{
    false, true,  true,  false, true,  true,  false, false,
    false, false, false, false, false, false, false, false};

inline const TetrominoShape Z_tetromino{
    true,  true,  false, false, false, true,  true,  false,
    false, false, false, false, false, false, false, false};

inline constexpr int gridWidth{10};
inline constexpr int gridHeight{20};
inline constexpr int gridWallThickness{16};
inline constexpr double gridPosX{
    screenWidth / 2.0 - (cellWidth * (gridWidth / 2.0)) - gridWallThickness};
inline constexpr double gridPosY{
    screenHeight / 2.0 - (cellHeight * (gridHeight / 2.0)) - gridWallThickness};
inline constexpr int gridWalls{4};
inline constexpr double initialTickTime{0.2};

}  // namespace constants
