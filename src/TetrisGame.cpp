#include "TetrisGame.h"

#include "Grid.h"
#include "Tetromino.h"
#include "TetrominoShape.h"
#include "colours.h"
#include "constants.h"
#include "flags.h"

#include <array>
#include <cstdint>
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
      grid_{screen, *this, colours::gridWalls},
      activeTetro{screen, grid_, {}, colours::gridBG, I_tetromino}, entities_{},
      possibleShapes_{}, upcomingShapes_{}, rng{}, tetroDist{}, score{0},
      linesClearedTotal{0}, linesClearedThisLevel{0}, level{1},
      displayBoxTitleFont(utl::createFont(constants::displayBoxFontPath,
                                          constants::displayBoxFontSize)),
      heldDisplayBox{
          screen,
          {constants::heldDisplayBoxPosX, constants::heldDisplayBoxPosY}},
      nextDisplayBox{
          screen,
          {constants::nextDisplayBoxPosX, constants::nextDisplayBoxPosY}},
      keyMap{}, canRotate{true}, rotateTimer{0.0}, canMove{true},
      moveTimer{0.0}, canSoftdrop{true}, softdropTimer{0.0}, running{true},
      scoreText{screen, renderer, displayBoxTitleFont, std::to_string(score),
                colours::titleText},
      levelText{screen, renderer, displayBoxTitleFont, std::to_string(level),
                colours::titleText},
      linesText{screen, renderer, displayBoxTitleFont,
                std::to_string(linesClearedTotal), colours::titleText}
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

    auto heldTitle{std::make_unique<utl::TextObject>(
        screen, renderer, displayBoxTitleFont, "HELD", colours::titleText)};
    heldTitle->recentreToEntityX(heldDisplayBox);
    heldTitle->setPos(
        {heldTitle->pos().x, heldDisplayBox.pos().y + heldDisplayBox.size().y
                                 + constants::displayBoxTitleBuffer});

    auto nextTitle{std::make_unique<utl::TextObject>(
        screen, renderer, displayBoxTitleFont, "NEXT", colours::titleText)};
    nextTitle->recentreToEntityX(nextDisplayBox);
    nextTitle->setPos(
        {nextTitle->pos().x, nextDisplayBox.pos().y + nextDisplayBox.size().y
                                 + constants::displayBoxTitleBuffer});

    auto scoreTitle{std::make_unique<utl::TextObject>(
        screen, renderer, displayBoxTitleFont, "SCORE", colours::titleText)};
    scoreTitle->recentreToEntityX(heldDisplayBox);
    scoreTitle->recentreToEntityY(grid_);

    scoreText.recentreToEntityX(heldDisplayBox);
    scoreText.setPos(
        {scoreText.pos().x, scoreTitle->pos().y + scoreTitle->size().y
                                + constants::displayBoxTitleBuffer});

    auto levelTitle{std::make_unique<utl::TextObject>(
        screen, renderer, displayBoxTitleFont, "LEVEL", colours::titleText)};
    levelTitle->recentreToEntityX(nextDisplayBox);
    levelTitle->recentreToEntityY(grid_);

    levelText.recentreToEntityX(nextDisplayBox);
    levelText.setPos(
        {levelText.pos().x, levelTitle->pos().y + levelTitle->size().y
                                + constants::displayBoxTitleBuffer});

    auto linesTitle{std::make_unique<utl::TextObject>(
        screen, renderer, displayBoxTitleFont, "LINES", colours::titleText)};
    linesTitle->recentreToEntityX(nextDisplayBox);
    linesTitle->setPos(
        {linesTitle->pos().x, levelText.pos().y + levelText.size().y
                                  + constants::displayBoxTitleBuffer});

    linesText.recentreToEntityX(nextDisplayBox);
    linesText.setPos(
        {linesText.pos().x, linesTitle->pos().y + linesTitle->size().y
                                + constants::displayBoxTitleBuffer});

    entities_.emplace_back(std::move(heldTitle));
    entities_.emplace_back(std::move(nextTitle));
    entities_.emplace_back(std::move(scoreTitle));
    entities_.emplace_back(std::move(levelTitle));
    entities_.emplace_back(std::move(linesTitle));
}

std::string
TetrisGame::handle_input(double, double,
                         std::array<bool, utl::KeyFlag::K_TOTAL>& keyState)
{
    utl::process_input(screen(), windowID(), keyState);

    // quitting
    if (keyState.at(utl::KeyFlag::QUIT)) {
        return flags::STAGES_MAP.at(flags::STAGES::QUIT);
    }

    if (keyState.at(utl::KeyFlag::K_ESCAPE)) {
        return flags::STAGES_MAP.at(flags::STAGES::TITLE_SCREEN);
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
            canRotate = false;
        } else if (keyState.at(utl::KeyFlag::K_LCTRL)
                   || keyState.at(utl::KeyFlag::K_Z)) {
            activeTetro.rotate(-1);
            canRotate = false;
        }
    }

    // moving
    if (canMove) {
        if (keyState.at(utl::KeyFlag::K_LEFT)) {
            activeTetro.move(-1);
            canMove = false;
        } else if (keyState.at(utl::KeyFlag::K_RIGHT)) {
            activeTetro.move(1);
            canMove = false;
        }
    }

    // soft-dropping
    if (canSoftdrop) {
        if (keyState.at(utl::KeyFlag::K_DOWN)) {
            activeTetro.soft_drop();
            canSoftdrop = false;
        }
    }

    return flags::STAGES_MAP.at(flags::STAGES::TETRIS);
}

std::string TetrisGame::update(double t, double dt)
{
    if (!running)
        return flags::STAGES_MAP.at(flags::STAGES::END_SCREEN);

    if (upcomingShapes_.size() < constants::shapeQueueMin)
        fillShapeQueue();

    if (!canRotate) {
        rotateTimer += dt;
        if (rotateTimer >= constants::rotateTimerMax) {
            canRotate = true;
            rotateTimer = 0.0;
        }
    }

    if (!canMove) {
        moveTimer += dt;
        if (moveTimer >= constants::moveTimerMax) {
            canMove = true;
            moveTimer = 0.0;
        }
    }

    if (!canSoftdrop) {
        softdropTimer += dt;
        if (softdropTimer >= constants::softdropTimerMax) {
            canSoftdrop = true;
            softdropTimer = 0.0;
        }
    }

    grid_.update(t, dt);
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
    scoreText.render(renderer());
    linesText.render(renderer());
    levelText.render(renderer());
    grid_.render(renderer());
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
        heldDisplayBox.updateShape(newHeld);
        activeTetro.reset(newActive);
    } else {
        heldDisplayBox.activate();
        heldDisplayBox.updateShape(newHeld);
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
    linesClearedThisLevel += linesCleared;
    linesClearedTotal += linesCleared;
    linesText.updateText(std::to_string(linesClearedTotal));
    linesText.recentreToEntityX(nextDisplayBox);

    int linePoints{};
    [[maybe_unused]] int tspinPoints{};

    switch (linesCleared) {
    case 1:
        linePoints = 100;
        break;
    case 2:
        linePoints = 200;
        break;
    case 3:
        linePoints = 500;
        break;
    case 4:
        linePoints = 800;
        break;
    }

    int scoreThisFrame{linePoints * level};
    score += scoreThisFrame;
    scoreText.updateText(std::to_string(score));
    scoreText.recentreToEntityX(heldDisplayBox);

    changeLevel();
}

void TetrisGame::notifyLoss()
{
    running = false;
}

void TetrisGame::changeLevel()
{
    if (linesClearedThisLevel < constants::linesPerLevel)
        return;

    if (level + 1 > constants::maxLevel)
        return;

    ++level;

    levelText.updateText(std::to_string(level));
    levelText.recentreToEntityX(nextDisplayBox);

    activeTetro.changeTickTime(1.0 - (0.0625 * level));

    linesClearedThisLevel -= constants::linesPerLevel;
}
