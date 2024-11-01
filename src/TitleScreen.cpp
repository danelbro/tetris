#include "TitleScreen.h"

#include "constants.h"
#include "flags.h"

#include <array>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>
#include <utl_TextObject.hpp>

TitleScreen::TitleScreen(utl::Box& screenRef, uint32_t windowID,
                         utl::Renderer& rendererRef)
    : utl::Stage{screenRef, windowID, rendererRef,
                 flags::STAGES_MAP.at(flags::STAGES::TITLE_SCREEN)},
      titleFont_{
          utl::createFont(constants::titleFontPath, constants::titleFontSize)},
      instructionsFont_{utl::createFont(constants::instructionsFontPath,
                                        constants::instructionsFontSize)},
      entities_{}
{
    auto title{
        std::make_unique<utl::TextObject>(screen(), renderer(), titleFont_)};
    title->loadFromRenderedText("TETRIS", {0xFF, 0xFF, 0xFF, 0xFF});
    title->recentre();
    title->setPos({title->pos().x, title->pos().y - title->size().y / 3});

    auto instructions{std::make_unique<utl::TextObject>(screen(), renderer(),
                                                        instructionsFont_)};
    instructions->loadFromRenderedText("Press Enter", {0xFF, 0xFF, 0xFF, 0xFF});
    instructions->recentre();
    instructions->setPos({instructions->pos().x,
                          instructions->pos().y + instructions->size().y});
    entities_.emplace_back(std::move(title));
    entities_.emplace_back(std::move(instructions));
}

std::string
TitleScreen::handle_input(double, double,
                          std::array<bool, utl::KeyFlag::K_TOTAL>& keyState)
{
    utl::process_input(screen(), windowID(), keyState);

    if (keyState.at(utl::KeyFlag::QUIT)
        || keyState.at(utl::KeyFlag::K_ESCAPE)) {
        return flags::STAGES_MAP.at(flags::STAGES::QUIT);
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
