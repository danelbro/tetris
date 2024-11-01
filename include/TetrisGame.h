// -*- C++ -*-
/// @file A Stage for running the main Tetris game loop.

#include <array>
#include <string>
#include <utl_Box.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>

class TetrisGame : public utl::Stage {
    TetrisGame(utl::Box& screen, uint32_t windowID, utl::Renderer& renderer);
    std::string handle_input(double t, double dt,
                             std::array<bool, utl::KeyFlag::K_TOTAL>& keyState);
    std::string update(double t, double dt);
    void render(double t, double dt);
};
