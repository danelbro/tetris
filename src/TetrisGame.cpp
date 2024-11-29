#include "TetrisGame.h"

#include "Grid.h"
#include "colours.h"
#include "flags.h"

#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <utl_Box.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>

TetrisGame::TetrisGame(utl::Box& screen, uint32_t windowID,
                       utl::Renderer& renderer)
    : utl::Stage{screen, windowID, renderer,
                 flags::STAGES_MAP.at(flags::STAGES::TETRIS)},
      grid{screen, colours::gridWalls}, entities_{}
{
    entities_.reserve(0xFF);
}

std::string
TetrisGame::handle_input(double, double,
                         std::array<bool, utl::KeyFlag::K_TOTAL>& keyState)
{
    utl::process_input(screen(), windowID(), keyState);

    if (keyState.at(utl::KeyFlag::QUIT) || keyState.at(utl::KeyFlag::K_ESCAPE)
        || keyState.at(utl::KeyFlag::K_ENTER)) {
        return flags::STAGES_MAP.at(flags::STAGES::QUIT);
    }

    return flags::STAGES_MAP.at(flags::STAGES::TETRIS);
}

std::string TetrisGame::update(double t, double dt)
{
    grid.update(t, dt);
    for (const auto& entity : entities_) {
        entity->update(t, dt);
    }
    return flags::STAGES_MAP.at(flags::STAGES::TETRIS);
}

void TetrisGame::render(double, double)
{
    utl::clearScreen(renderer());
    grid.render(renderer());
    for (const auto& entity : entities_) {
        entity->render(renderer());
    }
    utl::presentRenderer(renderer());
}
