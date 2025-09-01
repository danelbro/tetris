#include "EndScreen.h"
#include "colours.h"
#include "constants.h"
#include "flags.h"

#include <string>
#include <utl_Box.hpp>
#include <utl_Stage.hpp>
#include <utl_SDLInterface.hpp>

EndScreen::EndScreen(utl::Box& screen_space, uint32_t windowID,
                     utl::Renderer& rendererRef, const Grid& grid,
                     const ScoresPacket& scoresPacket)
    : utl::Stage{screen_space, windowID, rendererRef,
                 flags::STAGES_MAP.at(flags::STAGES::END_SCREEN)},
      grid_{screen_space, *this, colours::gridWalls},
      score_{scoresPacket.score}, level_{scoresPacket.level},
      lines_{scoresPacket.lines},
      smallFont_{utl::createFont(constants::displayBoxFontPath,
                                 constants::displayBoxFontSize)},
      largeFont_{utl::createFont(constants::instructionsFontPath,
                                 constants::instructionsFontSize)},
      gameText_{screen(), renderer(), largeFont_, "GAME",
                colours::instructionsText},
      overText_{screen(), renderer(), largeFont_, "OVER",
                colours::instructionsText},
      scoreTitle_{screen(), renderer(), smallFont_, "SCORE",
                  colours::instructionsText},
      scoreText_{screen(), renderer(), smallFont_, std::to_string(score_),
                 colours::instructionsText},
      levelTitle_{screen(), renderer(), smallFont_, "LEVEL",
                  colours::instructionsText},
      levelText_{screen(), renderer(), smallFont_, std::to_string(level_),
                 colours::instructionsText},
      linesTitle_{screen(), renderer(), smallFont_, "LINES",
                  colours::instructionsText},
      linesText_{screen(), renderer(), smallFont_, std::to_string(lines_),
                 colours::instructionsText}
{
    for (int y{0}; y < constants::gridHeight; ++y) {
        for (int x{0}; x < constants::gridWidth; ++x) {
            auto activeCell{grid.get(x, y)};
            grid_.setCellColour(x, y, activeCell.colour());
            grid_.setCellOpen(x, y,
                              activeCell.isOpen());  // not strictly necessary
        }
    }

    // position TextObjects
    gameText_.setPos({(grid_.pos().x * 2 / 3) - (gameText_.size().x / 2),
                      grid_.pos().y + constants::displayBoxWallsThickness
                          + (constants::displayBoxGridHeight
                             * constants::displayCellHeight / 2.0)});

    overText_.setPos({((grid_.pos().x + grid_.size().x)
                       + ((screen().w - (grid_.pos().x + grid_.size().x)) / 3))
                          - (overText_.size().x / 2),
                      grid_.pos().y + constants::displayBoxWallsThickness
                          + (constants::displayBoxGridHeight
                             * constants::displayCellHeight / 2.0)});

    scoreTitle_.recentreToEntityX(gameText_);
    scoreTitle_.recentreToEntityY(grid_);

    scoreText_.recentreToEntityX(scoreTitle_);
    scoreText_.setPos(
        {scoreText_.pos().x, scoreTitle_.pos().y + scoreTitle_.size().y
                                 + constants::displayBoxTitleBuffer});

    levelTitle_.recentreToEntityX(overText_);
    levelTitle_.recentreToEntityY(grid_);

    levelText_.recentreToEntityX(levelTitle_);
    levelText_.setPos(
        {levelText_.pos().x, levelTitle_.pos().y + levelTitle_.size().y
                                 + constants::displayBoxTitleBuffer});

    linesTitle_.recentreToEntityX(levelText_);
    linesTitle_.setPos(
        {linesTitle_.pos().x, levelText_.pos().y + levelText_.size().y
                                  + constants::displayBoxTitleBuffer});

    linesText_.recentreToEntityX(linesTitle_);
    linesText_.setPos(
        {linesText_.pos().x, linesTitle_.pos().y + linesTitle_.size().y
                                 + constants::displayBoxTitleBuffer});
}

std::string
EndScreen::handle_input(double, double,
                        std::array<bool, utl::KeyFlag::K_TOTAL>& keyState)
{
    utl::Box screenspace{screen()};
    utl::process_input(screenspace, windowID(), keyState);

    // quitting
    if (keyState.at(utl::KeyFlag::QUIT)) {
        return flags::STAGES_MAP.at(flags::STAGES::QUIT);
    }

    // return to title
    if (keyState.at(utl::KeyFlag::K_ESCAPE)
        || keyState.at(utl::KeyFlag::K_ENTER)) {
        return flags::STAGES_MAP.at(flags::STAGES::TITLE_SCREEN);
    }

    return flags::STAGES_MAP.at(flags::STAGES::END_SCREEN);
}

std::string EndScreen::update(double, double)
{
    return flags::STAGES_MAP.at(flags::STAGES::END_SCREEN);
}

void EndScreen::render(double, double)
{
    utl::clearScreen(renderer());
    grid_.render(renderer());
    gameText_.render(renderer());
    overText_.render(renderer());
    scoreTitle_.render(renderer());
    scoreText_.render(renderer());
    levelTitle_.render(renderer());
    levelText_.render(renderer());
    linesTitle_.render(renderer());
    linesText_.render(renderer());
    utl::presentRenderer(renderer());
}
