// -*- C++ -*- //
/// @file A Stage that displays a title screen and, depending on input, either
/// quits or goes into the main game stage

#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>
#include <vector>

/// A Stage that displays a title screen and, depending on input, either quits
/// or goes into the main game stage
class TitleScreen : public utl::Stage {
public:
    /// Constructs a TitleScreen
    /// @param screen
    TitleScreen(utl::Box& screen, uint32_t windowID, utl::Renderer& renderer);

    std::string
    handle_input(double t, double dt,
                 std::array<bool, utl::KeyFlag::K_TOTAL>& keyState) override;
    std::string update(double t, double dt) override;
    void render(double t, double dt) override;

private:
    utl::Font titleFont_;
    utl::Font instructionsFont_;

    std::vector<std::unique_ptr<utl::Entity>> entities_;
};
