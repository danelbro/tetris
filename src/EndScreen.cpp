#include "EndScreen.h"

#include "Grid.h"
#include "colours.h"
#include "constants.h"
#include "flags.h"

#include <string>
#include <utl_Application.hpp>
#include <utl_Box.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>

EndScreen::EndScreen(utl::Application& tetrisApp, Grid& grid,
                     const ScoresPacket& scoresPacket)
    : utl::Stage{}, app_{tetrisApp}, grid_{this, grid},
      score_{scoresPacket.score}, level_{scoresPacket.level},
      lines_{scoresPacket.lines},
      smallFont_{utl::createFont(constants::displayBoxFontPath,
                                 constants::displayBoxFontSize)},
      largeFont_{utl::createFont(constants::instructionsFontPath,
                                 constants::instructionsFontSize)},
      gameText_{this, &largeFont_, colours::instructionsText, "GAME"},
      overText_{this, &largeFont_, colours::instructionsText, "OVER"},
      scoreTitle_{this, &smallFont_, colours::instructionsText, "SCORE"},
      scoreText_{this, &smallFont_, colours::instructionsText,
                 std::to_string(score_)},
      levelTitle_{this, &smallFont_, colours::instructionsText, "LEVEL"},
      levelText_{this, &smallFont_, colours::instructionsText,
                 std::to_string(level_)},
      linesTitle_{this, &smallFont_, colours::instructionsText, "LINES"},
      linesText_{this, &smallFont_, colours::instructionsText,
                 std::to_string(lines_)}
{
    for (int y{0}; y < constants::gridHeight; ++y) {
        for (int x{0}; x < constants::gridWidth; ++x) {
            const Cell& activeCell{grid.get({x, y})};
            grid_.setCellColour({x, y}, activeCell.colour());
        }
    }

    // position TextObjects
    gameText_.set_pos({(grid_.pos().x * 2 / 3) - (gameText_.size().w / 2),
                       grid_.pos().y + constants::displayBoxWallsThickness
                           + (constants::displayBoxGridHeight
                              * constants::displayCellHeight / 2.0)});

    overText_.set_pos({((grid_.pos().x + grid_.size().w)
                        + ((screen().w - (grid_.pos().x + grid_.size().w)) / 3))
                           - (overText_.size().w / 2),
                       grid_.pos().y + constants::displayBoxWallsThickness
                           + (constants::displayBoxGridHeight
                              * constants::displayCellHeight / 2.0)});

    scoreTitle_.recentreX(gameText_);
    scoreTitle_.recentreY(grid_);

    scoreText_.recentreX(scoreTitle_);
    scoreText_.set_y_pos(scoreTitle_.pos().y + scoreTitle_.size().h
                         + constants::displayBoxTitleBuffer);

    levelTitle_.recentreX(overText_);
    levelTitle_.recentreY(grid_);

    levelText_.recentreX(levelTitle_);
    levelText_.set_y_pos(levelTitle_.pos().y + levelTitle_.size().h
                         + constants::displayBoxTitleBuffer);

    linesTitle_.recentreX(levelText_);
    linesTitle_.set_y_pos(levelText_.pos().y + levelText_.size().h
                          + constants::displayBoxTitleBuffer);

    linesText_.recentreX(linesTitle_);
    linesText_.set_y_pos(linesTitle_.pos().y + linesTitle_.size().h
                         + constants::displayBoxTitleBuffer);
}

std::string
EndScreen::handle_input(std::chrono::milliseconds, std::chrono::milliseconds,
                        std::array<bool, utl::KeyFlag::K_TOTAL>& keyState)
{
    utl::process_input(screen(), app_.window().ID(), keyState);

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

std::string EndScreen::update(std::chrono::milliseconds,
                              std::chrono::milliseconds)
{
    return flags::STAGES_MAP.at(flags::STAGES::END_SCREEN);
}

void EndScreen::render(std::chrono::milliseconds, std::chrono::milliseconds)
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

utl::Application& EndScreen::app()
{
    return app_;
}

utl::Box& EndScreen::screen()
{
    return app_.screen();
}

utl::Renderer& EndScreen::renderer()
{
    return app_.renderer();
}
