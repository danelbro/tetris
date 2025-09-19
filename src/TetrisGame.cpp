#include "TetrisGame.h"

#include "Grid.h"
#include "Tetromino.h"
#include "TetrominoShape.h"
#include "colours.h"
#include "constants.h"
#include "flags.h"

#include <array>
#include <cmath>
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
static int determineLineClearPoints(int linesCleared);
static int determineTSpinPoints(TetrisGame& game, int linesCleared);

TetrisGame::TetrisGame(utl::Application& tetris_app)
    : utl::Stage{}, app_{tetris_app},
      scoreText{this, &displayBoxTitleFont, colours::titleText,
                std::to_string(score)},
      levelText{this, &displayBoxTitleFont, colours::titleText,
                std::to_string(level)},
      linesText{this, &displayBoxTitleFont, colours::titleText,
                std::to_string(linesClearedTotal)},
      pauseText{this, &pauseFont, colours::titleText, std::string{"PAUSE"}}
{
    entities_.reserve(0xFF);

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

    pauseText.recentre(app_.screen());

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

    // pausing and quitting to title
    if (!keyMap.at(utl::KeyFlag::K_ESCAPE)) {
        if (keyState.at(utl::KeyFlag::K_ESCAPE)) {
            if (isPaused)
                return flags::STAGES_MAP.at(flags::STAGES::TITLE_SCREEN);
            isPaused = true;
            keyMap.at(utl::KeyFlag::K_ESCAPE) =
                keyState.at(utl::KeyFlag::K_ESCAPE);
        }
    } else {
        keyMap.at(utl::KeyFlag::K_ESCAPE) = keyState.at(utl::KeyFlag::K_ESCAPE);
    }

    // resuming
    if (!keyMap.at(utl::KeyFlag::K_ENTER)) {
        if (keyState.at(utl::KeyFlag::K_ENTER)) {
            isPaused = false;
            keyMap.at(utl::KeyFlag::K_ENTER) =
                keyState.at(utl::KeyFlag::K_ENTER);
        }
    } else {
        keyMap.at(utl::KeyFlag::K_ENTER) = keyState.at(utl::KeyFlag::K_ENTER);
    }

    if (isPaused)
        goto cleanup;

    // holding
    if (!keyMap.at(utl::KeyFlag::K_C) && !keyMap.at(utl::KeyFlag::K_LSHIFT)) {
        if (keyState.at(utl::KeyFlag::K_C)
            || keyState.at(utl::KeyFlag::K_LSHIFT)) {
            holdTetro();
            keyMap.at(utl::KeyFlag::K_C) = keyState.at(utl::KeyFlag::K_C);
            keyMap.at(utl::KeyFlag::K_LSHIFT) =
                keyState.at(utl::KeyFlag::K_LSHIFT);
            goto cleanup;
        }
    } else {
        keyMap.at(utl::KeyFlag::K_C) = keyState.at(utl::KeyFlag::K_C);
        keyMap.at(utl::KeyFlag::K_LSHIFT) = keyState.at(utl::KeyFlag::K_LSHIFT);
        goto cleanup;
    }

    // rotating
    if (canRotate) {
        if (keyState.at(utl::KeyFlag::K_UP) || keyState.at(utl::KeyFlag::K_X)) {
            activeTetro_.rotate(1);
            lastMove_ = flags::MOVE::ROTATE;
            canRotate = false;
            goto cleanup;
        } else if (keyState.at(utl::KeyFlag::K_LCTRL)
                   || keyState.at(utl::KeyFlag::K_Z)) {
            activeTetro_.rotate(-1);
            lastMove_ = flags::MOVE::ROTATE;
            canRotate = false;
            goto cleanup;
        }
    }

    // moving
    if (canMove) {
        if (keyState.at(utl::KeyFlag::K_LEFT)) {
            activeTetro_.move(-1);
            lastMove_ = flags::MOVE::MOVE;
            canMove = false;
            goto cleanup;
        } else if (keyState.at(utl::KeyFlag::K_RIGHT)) {
            activeTetro_.move(1);
            lastMove_ = flags::MOVE::MOVE;
            canMove = false;
            goto cleanup;
        }
    }

    // soft-dropping
    if (canSoftdrop) {
        if (keyState.at(utl::KeyFlag::K_DOWN)) {
            activeTetro_.soft_drop();
            lastMove_ = flags::MOVE::SOFTDROP;
            canSoftdrop = false;
            goto cleanup;
        }
    }

    // hard-dropping
    if (canHarddrop) {
        if (keyState.at(utl::KeyFlag::K_SPACE)) {
            hardDrop();
            canHarddrop = false;
            goto cleanup;
        }
    } else {
        if (!keyState.at(utl::KeyFlag::K_SPACE)) {
            canHarddrop = true;
            goto cleanup;
        }
    }

cleanup:
    return flags::STAGES_MAP.at(flags::STAGES::TETRIS);
}

std::string TetrisGame::update(double t, double dt)
{
    if (!isRunning)
        return flags::STAGES_MAP.at(flags::STAGES::END_SCREEN);

    if (isPaused)
        return flags::STAGES_MAP.at(flags::STAGES::TETRIS);

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

    timeSinceTick += dt;
    if (timeSinceTick >= tickTime_) {
        timeSinceTick = 0.0;
        activeTetro_.tick_down();
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
    if (isPaused)
        pauseText.render(renderer());
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
    lastMove_ = flags::MOVE::HARDDROP;
    hardDropCells = ghostPiece.origin().y - activeTetro_.topLeft().y;
    activeTetro_.setTopLeft(ghostPiece.origin());
    grid_.notifyBlockedTetro(activeTetro_);
}

void TetrisGame::resetActiveTetro()
{
    const TetrominoShape& newShape{upcomingShapes_.front()};
    upcomingShapes_.pop();
    nextDisplayBox.updateShape(upcomingShapes_.front());
    lastMove_ = flags::MOVE::NULLMOVE;

    activeTetro_.reset(newShape);
}

void TetrisGame::notifyBaked(int linesCleared)
{
    linesClearedThisLevel += linesCleared;
    linesClearedTotal += linesCleared;
    if (linesCleared >= 1) {
        linesText.updateText(std::to_string(linesClearedTotal));
        linesText.recentreX(nextDisplayBox);
        ++comboCount;
    } else {
        comboCount = -1;
    }

    int scoreThisFrame{0};

    int linePoints{determineLineClearPoints(linesCleared) * level};
    int tSpinPoints{determineTSpinPoints(*this, linesCleared) * level};
    int comboPoints{50 * comboCount * level};
    int softDropPoints{0};
    int hardDropPoints{2 * hardDropCells};

    bool difficultClear{};
    if (linesCleared == 4 || tSpinPoints > 0)
        difficultClear = true;
    else
        difficultClear = false;

    tSpinPoints > 0 ? scoreThisFrame = tSpinPoints
                    : scoreThisFrame = linePoints;

    if (difficultClear && difficultClearLastTime)
        scoreThisFrame = static_cast<int>(std::trunc(scoreThisFrame * 1.5));

    if (linesCleared >= 1)
        scoreThisFrame += softDropPoints + hardDropPoints + comboPoints;

    score += scoreThisFrame;

    scoreText.updateText(std::to_string(score));
    scoreText.recentreX(heldDisplayBox);

    changeLevel();
    hardDropCells = 0;
    difficultClearLastTime = difficultClear;
}

void TetrisGame::notifyLoss()
{
    isRunning = false;
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

flags::MOVE TetrisGame::lastMove() const
{
    return lastMove_;
}

const TetrominoShape& TetrisGame::getRandomShape()
{
    return possibleShapes_[tetroDist(rng.rng)];
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

    tickTime_ = 1.0 - (0.0625 * level);

    linesClearedThisLevel -= constants::linesPerLevel;
}

int determineLineClearPoints(int linesCleared)
{
    switch (linesCleared) {
    case 1:
        return 100;
    case 2:
        return 200;
    case 3:
        return 500;
    case 4:
        return 800;
    default:
        return 0;
    }
}

int determineTSpinPoints(TetrisGame& game, int linesCleared)
{
    if (game.activeTetro().shape() != T_tetromino
        || game.lastMove() != flags::MOVE::ROTATE)
        return 0;

    flags::TSpin tSpinType{game.activeTetro().checkTSpin()};
    int baseTSpinPoints{};
    int tSpinPoints{};

    switch (tSpinType) {
    case flags::TSpin::NOSPIN:
        return 0;
    case flags::TSpin::MINI :
        baseTSpinPoints = 100;
        break;
    case flags::TSpin::TSPIN :
        baseTSpinPoints = 400;
        break;
    }

    tSpinPoints = baseTSpinPoints * (linesCleared + 1);

    if (tSpinType == flags::TSpin::MINI && linesCleared == 2)
        tSpinPoints += 100;

    return tSpinPoints;
}
