// -*- C++ -*-
/// @file A Stage for running the main Tetris game loop.

#include <array>
#include <memory>
#include <string>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>
#include <vector>

class TetrisGame : public utl::Stage {
public:
    TetrisGame(utl::Box& screen, uint32_t windowID, utl::Renderer& renderer);
    std::string handle_input(double t, double dt,
                             std::array<bool, utl::KeyFlag::K_TOTAL>& keyState);
    std::string update(double t, double dt);
    void render(double t, double dt);

private:
    std::vector<std::unique_ptr<utl::Entity>> entities_;
};
