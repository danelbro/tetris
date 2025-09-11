#include "TitleScreen.h"

#include "colours.h"
#include "flags.h"

#include <array>
#include <memory>
#include <string>
#include <utility>
#include <utl_Application.hpp>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>
#include <utl_TextObject.hpp>

TitleScreen::TitleScreen(utl::Application& app) : utl::Stage{}, owner_{app}
{
    auto title{std::make_unique<utl::TextObject>(this, &titleFont_,
                                                 colours::titleText, "TETRIS")};
    title->recentre(owner_.screen());
    title->move_y_pos(-(title->size().h / 3));

    auto instructions{std::make_unique<utl::TextObject>(
        this, &instructionsFont_, colours::instructionsText, "Press Enter")};
    instructions->recentre(owner_.screen());
    instructions->move_y_pos(instructions->size().h);

    entities_.emplace_back(std::move(title));
    entities_.emplace_back(std::move(instructions));

    utl::setRendererDrawColour(renderer(), colours::background);
}

std::string
TitleScreen::handle_input(double, double,
                          std::array<bool, utl::KeyFlag::K_TOTAL>& keyState)
{
    utl::process_input(screen(), owner_.window().ID(), keyState);

    if (keyState.at(utl::KeyFlag::QUIT)) {
        return flags::STAGES_MAP.at(flags::STAGES::QUIT);
    }

    if (keyState.at(utl::KeyFlag::K_ENTER)) {
        return flags::STAGES_MAP.at(flags::STAGES::TETRIS);
    }

    return flags::STAGES_MAP.at(flags::STAGES::TITLE_SCREEN);
}

std::string TitleScreen::update(double, double)
{
    return flags::STAGES_MAP.at(flags::STAGES::TITLE_SCREEN);
}

void TitleScreen::render(double, double)
{
    utl::clearScreen(renderer());
    for (const auto& entity : entities_) {
        entity->render(renderer());
    }
    utl::presentRenderer(renderer());
}

utl::Application& TitleScreen::app()
{
    return owner_;
}

utl::Box& TitleScreen::screen()
{
    return owner_.screen();
}

utl::Renderer& TitleScreen::renderer()
{
    return owner_.renderer();
}
