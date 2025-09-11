#include "TetrisGame.h"

#include "TetrominoShape.h"
#include "colours.h"
#include "constants.h"
#include "flags.h"

#include <array>
#include <memory>
#include <string>
#include <utl_Application.hpp>
#include <utl_Box.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>
#include <utl_TextObject.hpp>
#include <utl_random.hpp>

static const utl::Vec2d newpos{
    constants::gridPosX + constants::gridWallThickness,
    constants::gridPosY + constants::gridWallThickness};

TetrisGame::TetrisGame(utl::Application& tetris_app)
    : utl::Stage{}, app_{tetris_app},
      displayBoxTitleFont{utl::createFont(constants::displayBoxFontPath,
                                          constants::displayBoxFontSize)},
      scoreText{this, &displayBoxTitleFont, colours::titleText,
                std::to_string(score)},
      levelText{this, &displayBoxTitleFont, colours::titleText,
                std::to_string(level)},
      linesText{this, &displayBoxTitleFont, colours::titleText,
                std::to_string(linesClearedTotal)}
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

    rng.rng().seed();
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
        this, &displayBoxTitleFont, colours::titleText, "HELD")};
    heldTitle->recentreX(heldDisplayBox);
    heldTitle->set_y_pos(heldDisplayBox.pos().y + heldDisplayBox.size().h
                         + constants::displayBoxTitleBuffer);

    auto nextTitle{std::make_unique<utl::TextObject>(
        this, &displayBoxTitleFont, colours::titleText, "NEXT")};
    nextTitle->recentreX(nextDisplayBox);
    nextTitle->set_y_pos(nextDisplayBox.pos().y + nextDisplayBox.size().h
                         + constants::displayBoxTitleBuffer);

    auto scoreTitle{std::make_unique<utl::TextObject>(
        this, &displayBoxTitleFont, colours::titleText, "SCORE")};
    scoreTitle->recentreX(heldDisplayBox);
    scoreTitle->recentreY(grid_);

    scoreText.recentreX(heldDisplayBox);
    scoreText.set_y_pos(scoreTitle->pos().y + scoreTitle->size().h
                        + constants::displayBoxTitleBuffer);

    auto levelTitle{std::make_unique<utl::TextObject>(
        this, &displayBoxTitleFont, colours::titleText, "LEVEL")};
    levelTitle->recentreX(nextDisplayBox);
    levelTitle->recentreY(grid_);

    levelText.recentreX(nextDisplayBox);
    levelText.set_y_pos(levelTitle->pos().y + levelTitle->size().h
                        + constants::displayBoxTitleBuffer);

    auto linesTitle{std::make_unique<utl::TextObject>(
        this, &displayBoxTitleFont, colours::titleText, "LINES")};
    linesTitle->recentreX(nextDisplayBox);
    linesTitle->set_y_pos(levelText.pos().y + levelText.size().h
                          + constants::displayBoxTitleBuffer);

    linesText.recentreX(nextDisplayBox);
    linesText.set_y_pos(linesTitle->pos().y + linesTitle->size().h
                        + constants::displayBoxTitleBuffer);

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
    utl::process_input(screen(), app_.window().ID(), keyState);

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
            activeTetro_.rotate(1);
            canRotate = false;
        } else if (keyState.at(utl::KeyFlag::K_LCTRL)
                   || keyState.at(utl::KeyFlag::K_Z)) {
            activeTetro_.rotate(-1);
            canRotate = false;
        }
    }

    // moving
    if (canMove) {
        if (keyState.at(utl::KeyFlag::K_LEFT)) {
            activeTetro_.move(-1);
            canMove = false;
        } else if (keyState.at(utl::KeyFlag::K_RIGHT)) {
            activeTetro_.move(1);
            canMove = false;
        }
    }

    // soft-dropping
    if (canSoftdrop) {
        if (keyState.at(utl::KeyFlag::K_DOWN)) {
            activeTetro_.soft_drop();
            canSoftdrop = false;
        }
    }

    // hard-dropping
    if (canHarddrop) {
        if (keyState.at(utl::KeyFlag::K_SPACE)) {
            hardDrop();
            canHarddrop = false;
        }
    } else {
        if (!keyState.at(utl::KeyFlag::K_SPACE))
            canHarddrop = true;
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
    activeTetro_.update(t, dt);
    ghostPiece.update(t, dt);
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
    activeTetro_.render(renderer());
    ghostPiece.render(renderer());
    for (const auto& entity : entities_) {
        entity->render(renderer());
    }
    utl::presentRenderer(renderer());
}

utl::Application& TetrisGame::app()
{
    return app_;
}

utl::Box& TetrisGame::screen()
{
    return app_.screen();
}

utl::Renderer& TetrisGame::renderer()
{
    return app_.renderer();
}

void TetrisGame::holdTetro()
{
    TetrominoShape newHeld{activeTetro_.shape()};

    if (heldDisplayBox.isActivated()) {
        TetrominoShape newActive{heldDisplayBox.activeShape()};
        heldDisplayBox.updateShape(newHeld);
        activeTetro_.reset(newActive);
    } else {
        heldDisplayBox.activate();
        heldDisplayBox.updateShape(newHeld);
        resetActiveTetro();
    }
}

void TetrisGame::hardDrop()
{
    activeTetro_.setTopLeft(ghostPiece.origin());
    grid_.notifyBlockedTetro(activeTetro_);
}

void TetrisGame::resetActiveTetro()
{
    const TetrominoShape& newShape{upcomingShapes_.front()};
    upcomingShapes_.pop();
    nextDisplayBox.updateShape(upcomingShapes_.front());

    activeTetro_.reset(newShape);
}

void TetrisGame::notifyScored(int linesCleared)
{
    linesClearedThisLevel += linesCleared;
    linesClearedTotal += linesCleared;
    linesText.updateText(std::to_string(linesClearedTotal));
    linesText.recentreX(nextDisplayBox);

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
    scoreText.recentreX(heldDisplayBox);

    changeLevel();
}

void TetrisGame::notifyLoss()
{
    running = false;
}

Grid& TetrisGame::grid()
{
    return grid_;
}

const Tetromino& TetrisGame::activeTetro() const
{
    return activeTetro_;
}

int TetrisGame::getScore() const
{
    return score;
}

int TetrisGame::getLines() const
{
    return linesClearedTotal;
}

int TetrisGame::getLevel() const
{
    return level;
}

const TetrominoShape& TetrisGame::getRandomShape()
{
    return possibleShapes_[tetroDist(rng.rng())];
}

void TetrisGame::fillShapeQueue()
{
    while (upcomingShapes_.size() < constants::shapeQueueMax) {
        upcomingShapes_.emplace(getRandomShape());
    }
}

void TetrisGame::changeLevel()
{
    if (linesClearedThisLevel < constants::linesPerLevel)
        return;

    if (level + 1 > constants::maxLevel)
        return;

    ++level;

    levelText.updateText(std::to_string(level));
    levelText.recentreX(nextDisplayBox);

    activeTetro_.changeTickTime(1.0 - (0.0625 * level));

    linesClearedThisLevel -= constants::linesPerLevel;
}
