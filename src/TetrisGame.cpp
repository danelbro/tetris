#include "TetrisGame.h"

#include "Grid.h"
#include "Tetromino.h"
#include "TetrominoShape.h"
#include "colours.h"
#include "constants.h"
#include "flags.h"

#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <utl_Box.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>

static const utl::Vec2d newpos{
    constants::gridPosX + constants::gridWallThickness,
    constants::gridPosY + constants::gridWallThickness};

TetrisGame::TetrisGame(utl::Box& screen, uint32_t windowID,
                       utl::Renderer& renderer)
    : utl::Stage{screen, windowID, renderer,
                 flags::STAGES_MAP.at(flags::STAGES::TETRIS)},
      grid{screen, *this, colours::gridWalls},
      activeTetro{screen, grid, {}, colours::gridBG, I_tetromino}, entities_{},
      possibleShapes_{}, upcomingShapes_{}, rng{}, tetroDist{}, score{0}
{
    entities_.reserve(0xFF);
    possibleShapes_.reserve(constants::tetrominoes);

    possibleShapes_.emplace_back(I_tetromino);
    possibleShapes_.emplace_back(O_tetromino);
    possibleShapes_.emplace_back(T_tetromino);
    possibleShapes_.emplace_back(J_tetromino);
    possibleShapes_.emplace_back(L_tetromino);
    possibleShapes_.emplace_back(S_tetromino);
    possibleShapes_.emplace_back(Z_tetromino);

    std::random_device dev;
    rng = std::mt19937{dev()};
    tetroDist = std::uniform_int_distribution<std::mt19937::result_type>{
        0, constants::tetrominoes - 1};

    fillShapeQueue();
    resetActiveTetro();
}

std::string
TetrisGame::handle_input(double, double,
                         std::array<bool, utl::KeyFlag::K_TOTAL>& keyState)
{
    utl::process_input(screen(), windowID(), keyState);

    if (keyState.at(utl::KeyFlag::QUIT)
        || keyState.at(utl::KeyFlag::K_ESCAPE)) {
        return flags::STAGES_MAP.at(flags::STAGES::QUIT);
    }
    if (keyState.at(utl::KeyFlag::K_UP) || keyState.at(utl::KeyFlag::K_X)) {
        activeTetro.rotate(1);
    } else if (keyState.at(utl::KeyFlag::K_LCTRL)
               || keyState.at(utl::KeyFlag::K_Z)) {
        activeTetro.rotate(-1);
    }

    if (keyState.at(utl::KeyFlag::K_LEFT)) {
        activeTetro.move(-1);
    } else if (keyState.at(utl::KeyFlag::K_RIGHT)) {
        activeTetro.move(1);
    }

    if (keyState.at(utl::KeyFlag::K_DOWN)) {
        activeTetro.soft_drop();
    }

    return flags::STAGES_MAP.at(flags::STAGES::TETRIS);
}

std::string TetrisGame::update(double t, double dt)
{
    if (upcomingShapes_.size() < constants::shapeQueueMin)
        fillShapeQueue();

    grid.update(t, dt);
    activeTetro.update(t, dt);
    for (const auto& entity : entities_) {
        entity->update(t, dt);
    }
    return flags::STAGES_MAP.at(flags::STAGES::TETRIS);
}

void TetrisGame::render(double, double)
{
    utl::clearScreen(renderer());
    grid.render(renderer());
    activeTetro.render(renderer());
    for (const auto& entity : entities_) {
        entity->render(renderer());
    }
    utl::presentRenderer(renderer());
}

void TetrisGame::resetActiveTetro()
{
    const TetrominoShape& newShape{upcomingShapes_.front()};
    upcomingShapes_.pop();

    activeTetro.reset(newShape);
}

const TetrominoShape& TetrisGame::getRandomShape()
{
    return possibleShapes_[tetroDist(rng)];
}

void TetrisGame::fillShapeQueue()
{
    while (upcomingShapes_.size() < constants::shapeQueueMax) {
        upcomingShapes_.emplace(getRandomShape());
    }
}

void TetrisGame::notifyScored(int linesCleared)
{
    if (linesCleared == constants::tetrisLines)
        std::cout << "Tetris! ";

    int scoreThisFrame{ linesCleared * 1000 };
    score += scoreThisFrame;

    std::cout << "Gained " << scoreThisFrame << " points! Total score: "
        << score << '\n';
}