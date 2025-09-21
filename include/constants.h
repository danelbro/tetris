// -*- C++ -*-
/// @file Contains global constants for configuration

#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <utl_SDLInterface.hpp>

namespace constants {
using namespace std::chrono_literals;

inline const std::string tetrisTitle{"Tetris"};
inline const std::string tetrisVersion{"1.0.0"};
inline const std::string tetrisIdentifier{"com.tetris.tetris"};
inline constexpr int screenWidth{1024};
inline constexpr int screenHeight{768};
inline constexpr unsigned sdlFlags{SDL_INIT_VIDEO};
inline constexpr unsigned windowFlags{0};

inline constexpr int shapeWidth{4};
inline constexpr int shapeHeight{4};
inline constexpr size_t minos{4};
inline constexpr size_t rotations{4};
inline constexpr size_t tetrominoes{7};
inline constexpr size_t filledCornersForTSpin{3};
inline constexpr int frontCornersForTSpin{2};
inline constexpr int tests{5};
inline constexpr size_t shapeQueueMin{2};
inline constexpr size_t shapeQueueMax{10};
inline constexpr int pointsPerSoftDropCell{1};
inline constexpr int pointsPerHardDropCell{2};
inline constexpr int baseComboPoints{50};
inline constexpr int singleLines{1};
inline constexpr int doubleLines{2};
inline constexpr int tripleLines{3};
inline constexpr int tetrisLines{4};
inline constexpr int singlePoints{100};
inline constexpr int doublePoints{200};
inline constexpr int triplePoints{500};
inline constexpr int tetrisPoints{800};
inline constexpr int miniTSpinBasePoints{100};
inline constexpr int TSpinBasePoints{400};

inline constexpr std::chrono::seconds initialTickTime{1s};
inline constexpr std::chrono::milliseconds rotateTimerMax{100ms};
inline constexpr std::chrono::milliseconds moveTimerMax{50ms};
inline constexpr std::chrono::milliseconds softdropTimerMax{50ms};
inline constexpr std::chrono::milliseconds dissolveTODefaultVisibleTime{1000ms};

inline constexpr int maxLevel{15};
inline constexpr int linesPerLevel{10};

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
inline constexpr double gridRightX{gridPosX + (gridWallThickness * 2)
                                   + (gridWidth * cellWidth)};

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
       / 2) - 50};
inline constexpr double heldDisplayBoxPosY{gridPosY + gridWallThickness};

inline constexpr double nextDisplayBoxPosX{
    gridRightX + ((screenWidth - gridRightX) / 3.0)
    - (static_cast<double>((displayBoxWallsThickness * 2)
                           + (displayCellWidth * displayBoxGridWidth))
       / 2) + 50};
inline constexpr double nextDisplayBoxPosY{gridPosY + gridWallThickness};

inline constexpr double levelUpYShift{100};
inline constexpr double tSpinYShift{150};
inline constexpr double lineClearYShift{200};
inline constexpr double comboYShift{250};
inline constexpr double backToBackYShift{300};

}  // namespace constants
