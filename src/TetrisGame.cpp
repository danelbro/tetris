#include "TetrisGame.h"

#include "Grid.h"
#include "Tetromino.h"
#include "TetrominoShape.h"
#include "colours.h"
#include "constants.h"
#include "flags.h"

#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <utl_Box.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>
#include <utl_TextObject.hpp>

static const utl::Vec2d newpos{
    constants::gridPosX + constants::gridWallThickness,
    constants::gridPosY + constants::gridWallThickness};

TetrisGame::TetrisGame(utl::Box& screen, uint32_t windowID,
                       utl::Renderer& renderer)
    : utl::Stage{screen, windowID, renderer,
                 flags::STAGES_MAP.at(flags::STAGES::TETRIS)},
      grid{screen, *this, colours::gridWalls},
      activeTetro{screen, grid, {}, colours::gridBG, I_tetromino}, entities_{},
      possibleShapes_{}, upcomingShapes_{}, rng{}, tetroDist{}, score{0},
      displayBoxTitleFont(utl::createFont(constants::displayBoxFontPath,
                                          constants::displayBoxFontSize)),
    heldDisplayBox{ screen, {constants::heldDisplayBoxPosX,
    constants::heldDisplayBoxPosY} },
    nextDisplayBox{ screen, {constants::nextDisplayBoxPosX,
    constants::nextDisplayBoxPosY} },
    keyMap{}, canRotate{ true },
    canMove{ true },  canSoftdrop{true}
{
    entities_.reserve(0xFF);
    possibleShapes_.reserve(constants::tetrominoes);

    possibleShapes_.emplace_back(I_tetromino);
    possibleShapes_.emplace_back(O_tetromino);
    possibleShapes_.emplace_back(T_tetromino);
    possibleShapes_.emplace_back(J_tetromino);
    possibleShapes_.emplace_back(L_tetromino);
    possibleShapes_.emplace_back(S_tetromino);
    possibleShapes_.emplace_back(Z_tetromino);

    std::random_device dev;
    rng = std::mt19937{dev()};
    tetroDist = std::uniform_int_distribution<std::mt19937::result_type>{
        0, constants::tetrominoes - 1};

    fillShapeQueue();
    nextDisplayBox.activate();
    resetActiveTetro();

    for (int keyInt{utl::KeyFlag::K_LEFT}; keyInt != utl::KeyFlag::K_TOTAL;
         ++keyInt) {
        keyMap[static_cast<utl::KeyFlag>(keyInt)] = false;
    }

    auto heldTitle{std::make_unique<utl::TextObject>(screen, renderer,
                                                     displayBoxTitleFont)};
    heldTitle->loadFromRenderedText("HELD", colours::titleText);
    heldTitle->recentre();
    double heldXPos{
        (heldDisplayBox.pos().x
         + ((constants::displayCellWidth * constants::displayBoxGridWidth
             + (constants::displayBoxWallsThickness * 2))
            / 2.0))
        - (heldTitle->size().x / 2.0)};
    double heldYPos{
        heldDisplayBox.pos().y
        + (constants::displayCellHeight * constants::displayBoxGridHeight)
        + (constants::displayBoxWallsThickness * 2)
        + constants::displayBoxTitleBuffer};
    heldTitle->setPos({heldXPos, heldYPos});

    auto nextTitle{ std::make_unique<utl::TextObject>(screen,renderer,displayBoxTitleFont) };
    nextTitle->loadFromRenderedText("NEXT", colours::titleText);
    nextTitle->recentre();
    double nextXPos{
        (nextDisplayBox.pos().x
            + ((constants::displayCellWidth * constants::displayBoxGridWidth
                + (constants::displayBoxWallsThickness * 2))
                / 2.0))
            - (nextTitle->size().x / 2.0) };
    double nextYPos{
        nextDisplayBox.pos().y
        + (constants::displayCellHeight * constants::displayBoxGridHeight)
        + (constants::displayBoxWallsThickness * 2)
        + constants::displayBoxTitleBuffer };
    nextTitle->setPos({nextXPos, nextYPos});

    entities_.emplace_back(std::move(heldTitle));
    entities_.emplace_back(std::move(nextTitle));
}

std::string
TetrisGame::handle_input(double, double,
                         std::array<bool, utl::KeyFlag::K_TOTAL>& keyState)
{
    utl::process_input(screen(), windowID(), keyState);

    // quitting
    if (keyState.at(utl::KeyFlag::QUIT)
        || keyState.at(utl::KeyFlag::K_ESCAPE)) {
        return flags::STAGES_MAP.at(flags::STAGES::QUIT);
    }

    // holding
    if (!keyMap.at(utl::KeyFlag::K_C) && !keyMap.at(utl::KeyFlag::K_LSHIFT)) {
        if (keyState.at(utl::KeyFlag::K_C)
            || keyState.at(utl::KeyFlag::K_LSHIFT)) {
            holdTetro();
            keyMap.at(utl::KeyFlag::K_C) = keyState.at(utl::KeyFlag::K_C);
            keyMap.at(utl::KeyFlag::K_LSHIFT) =
                keyState.at(utl::KeyFlag::K_LSHIFT);
        }
    } else {
        keyMap.at(utl::KeyFlag::K_C) = keyState.at(utl::KeyFlag::K_C);
        keyMap.at(utl::KeyFlag::K_LSHIFT) = keyState.at(utl::KeyFlag::K_LSHIFT);
    }

    // rotating
    if (canRotate) {
        if (keyState.at(utl::KeyFlag::K_UP) || keyState.at(utl::KeyFlag::K_X)) {
            activeTetro.rotate(1);
        } else if (keyState.at(utl::KeyFlag::K_LCTRL)
                   || keyState.at(utl::KeyFlag::K_Z)) {
            activeTetro.rotate(-1);
        }
    }

    // moving
    if (canMove) {
        if (keyState.at(utl::KeyFlag::K_LEFT)) {
            activeTetro.move(-1);
        } else if (keyState.at(utl::KeyFlag::K_RIGHT)) {
            activeTetro.move(1);
        }
    }

    // soft-dropping
    if (canSoftdrop) {
        if (keyState.at(utl::KeyFlag::K_DOWN)) {
            activeTetro.soft_drop();
        }
    }

    return flags::STAGES_MAP.at(flags::STAGES::TETRIS);
}

std::string TetrisGame::update(double t, double dt)
{
    if (upcomingShapes_.size() < constants::shapeQueueMin)
        fillShapeQueue();

    grid.update(t, dt);
    activeTetro.update(t, dt);
    for (const auto& entity : entities_) {
        entity->update(t, dt);
    }
    return flags::STAGES_MAP.at(flags::STAGES::TETRIS);
}

void TetrisGame::render(double, double)
{
    utl::clearScreen(renderer());
    heldDisplayBox.render(renderer());
    nextDisplayBox.render(renderer());
    grid.render(renderer());
    activeTetro.render(renderer());
    for (const auto& entity : entities_) {
        entity->render(renderer());
    }
    utl::presentRenderer(renderer());
}

void TetrisGame::resetActiveTetro()
{
    const TetrominoShape& newShape{upcomingShapes_.front()};
    upcomingShapes_.pop();
    nextDisplayBox.updateShape(upcomingShapes_.front());

    activeTetro.reset(newShape);
}

void TetrisGame::holdTetro()
{
    TetrominoShape newHeld{activeTetro.shape()};

    if (heldDisplayBox.isActivated()) {
        TetrominoShape newActive{heldDisplayBox.activeShape()};
        heldDisplayBox.activate();
        heldDisplayBox.updateShape(newHeld);
        heldDisplayBox.activate();
        activeTetro.reset(newActive);
    } else {
        heldDisplayBox.activate();
        heldDisplayBox.updateShape(newHeld);
        heldDisplayBox.activate();
        resetActiveTetro();
    }
}

const TetrominoShape& TetrisGame::getRandomShape()
{
    return possibleShapes_[tetroDist(rng)];
}

void TetrisGame::fillShapeQueue()
{
    while (upcomingShapes_.size() < constants::shapeQueueMax) {
        upcomingShapes_.emplace(getRandomShape());
    }
}

void TetrisGame::notifyScored(int linesCleared)
{
    if (linesCleared == constants::tetrisLines)
        std::cout << "Tetris! ";

    int scoreThisFrame{linesCleared * 1000};
    score += scoreThisFrame;

    std::cout << "Gained " << scoreThisFrame
              << " points! Total score: " << score << '\n';
}
