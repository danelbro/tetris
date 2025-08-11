#include "TetrisGame.h"

#include "Grid.h"
#include "Tetromino.h"
#include "colours.h"
#include "constants.h"
#include "flags.h"

#include <array>
#include <cstdint>
#include <memory>
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
      grid{screen, colours::gridWalls},
      testTetro{screen,
                grid,
                {(constants::gridWidth / 2) - 2, 0},
                colours::S_tetrominoCol,
                constants::S_tetromino},
      entities_{}
{
    entities_.reserve(0xFF);
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
        testTetro.rotate(1);
    } else if (keyState.at(utl::KeyFlag::K_LCTRL)
               || keyState.at(utl::KeyFlag::K_Z)) {
        testTetro.rotate(-1);
    } else if (keyState.at(utl::KeyFlag::K_LEFT)) {
        testTetro.move(-1);
    } else if (keyState.at(utl::KeyFlag::K_RIGHT)) {
        testTetro.move(1);
    } else if (keyState.at(utl::KeyFlag::K_DOWN)) {
        testTetro.soft_drop();
    }

    return flags::STAGES_MAP.at(flags::STAGES::TETRIS);
}

std::string TetrisGame::update(double t, double dt)
{
    grid.update(t, dt);
    testTetro.update(t, dt);
    for (const auto& entity : entities_) {
        entity->update(t, dt);
    }
    return flags::STAGES_MAP.at(flags::STAGES::TETRIS);
}

void TetrisGame::render(double, double)
{
    utl::clearScreen(renderer());
    grid.render(renderer());
    testTetro.render(renderer());
    for (const auto& entity : entities_) {
        entity->render(renderer());
    }
    utl::presentRenderer(renderer());
}
