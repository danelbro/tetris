// -*- C++ -*- //
/// @file A Stage that displays a title screen and, depending on input, either
/// quits or goes into the main game stage

#pragma once

#include "constants.h"
#include "flags.h"

#include <array>
#include <memory>
#include <string>
#include <utl_Application.hpp>
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
    TitleScreen(utl::Application& tetrisApp);

    std::string handle_input(
        double t, double dt,
        std::array<bool, utl::KeyFlag::K_TOTAL>& keyState) override final;
    std::string update(double t, double dt) override final;
    void render(double t, double dt) override final;

    utl::Application& app() override final;
    utl::Box& screen() override final;
    utl::Renderer& renderer() override final;

private:
    std::string label{flags::STAGES_MAP.at(flags::STAGES::TITLE_SCREEN)};
    utl::Application& owner_;
    utl::Font titleFont_{
        utl::createFont(constants::titleFontPath, constants::titleFontSize)};
    utl::Font instructionsFont_{utl::createFont(
        constants::instructionsFontPath, constants::instructionsFontSize)};
    std::vector<std::unique_ptr<utl::Entity>> entities_{};
};
