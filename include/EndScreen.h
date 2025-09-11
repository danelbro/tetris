// -*- C++ -*-

#pragma once

#include "InertGrid.h"
#include "flags.h"

#include <string>
#include <utl_Box.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>
#include <utl_TextObject.hpp>

class Grid;

struct ScoresPacket {
    int score;
    int level;
    int lines;
};

class EndScreen : public utl::Stage {
public:
    EndScreen(utl::Application& tetrisApp, const Grid& grid,
              const ScoresPacket& scoresPacket);

    std::string handle_input(
        double t, double dt,
        std::array<bool, utl::KeyFlag::K_TOTAL>& keyState) override final;
    std::string update(double t, double dt) override final;
    void render(double t, double dt) override final;

    utl::Application& app() override final;
    utl::Box& screen() override final;
    utl::Renderer& renderer() override final;

private:
    std::string label{flags::STAGES_MAP.at(flags::STAGES::END_SCREEN)};
    utl::Application& app_;
    InertGrid grid_;
    int score_;
    int level_;
    int lines_;

    utl::Font smallFont_;
    utl::Font largeFont_;

    utl::TextObject gameText_;
    utl::TextObject overText_;
    utl::TextObject scoreTitle_;
    utl::TextObject scoreText_;
    utl::TextObject levelTitle_;
    utl::TextObject levelText_;
    utl::TextObject linesTitle_;
    utl::TextObject linesText_;
};
