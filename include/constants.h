// -*- C++ -*-
/// @file Contains global constants for configuration

#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <utl_SDLInterface.hpp>

namespace constants {
inline constexpr int shapeWidth{4};
inline constexpr int shapeHeight{4};

inline constexpr size_t minos{4};
inline constexpr size_t rotations{4};
inline constexpr size_t tetrominoes{7};
inline constexpr size_t filledCornersForTSpin{3};
inline constexpr int frontCornersForTSpin{2};
inline constexpr int tests{5};
inline constexpr int tetrisLines{4};

inline constexpr size_t shapeQueueMin{2};
inline constexpr size_t shapeQueueMax{10};

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
inline constexpr int32_t pauseFontSize{192};

inline constexpr int cellWidth{32};
inline constexpr int cellHeight{32};

inline constexpr int gridWidth{10};
inline constexpr int gridHeight{20};
inline constexpr int gridWallThickness{16};
inline constexpr double gridPosX{
    screenWidth / 2.0 - (cellWidth * (gridWidth / 2.0)) - gridWallThickness};
inline constexpr double gridPosY{
    screenHeight / 2.0 - (cellHeight * (gridHeight / 2.0)) - gridWallThickness};
inline constexpr int gridWalls{4};
inline constexpr double initialTickTime{1.0};

inline constexpr int displayBoxWalls{4};
inline constexpr int displayBoxGridWidth{shapeWidth + 2};
inline constexpr int displayBoxGridHeight{shapeHeight + 2};
inline constexpr int displayCellWidth{cellWidth / 2};
inline constexpr int displayCellHeight{cellHeight / 2};
inline constexpr int displayBoxWallsThickness{gridWallThickness / 2};
inline constexpr int32_t displayBoxFontSize{32};
inline const std::filesystem::path displayBoxFontPath{
    "data/Silkscreen-Regular.ttf"};
inline constexpr int32_t displayBoxTitleBuffer{10};

inline constexpr double heldDisplayBoxPosX{
    (gridPosX * 2 / 3)
    - (static_cast<double>((displayBoxWallsThickness * 2)
                           + (displayCellWidth * displayBoxGridWidth))
       / 2)};
inline constexpr double heldDisplayBoxPosY{gridPosY + gridWallThickness};

inline constexpr double gridRightX{gridPosX + (gridWallThickness * 2)
                                   + (gridWidth * cellWidth)};
inline constexpr double nextDisplayBoxPosX{
    gridRightX + ((screenWidth - gridRightX) / 3.0)
    - (static_cast<double>((displayBoxWallsThickness * 2)
                           + (displayCellWidth * displayBoxGridWidth))
       / 2)};
inline constexpr double nextDisplayBoxPosY{gridPosY + gridWallThickness};

inline constexpr double rotateTimerMax{0.10};
inline constexpr double moveTimerMax{0.05};
inline constexpr double softdropTimerMax{0.05};

inline constexpr int maxLevel{15};
inline constexpr int linesPerLevel{10};
}  // namespace constants
