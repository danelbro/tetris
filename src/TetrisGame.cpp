#include "TetrisGame.h"

#include "DissolveTextObject.h"
#include "Grid.h"
#include "Tetromino.h"
#include "TetrominoShape.h"
#include "colours.h"
#include "constants.h"
#include "flags.h"

#include <array>
#include <chrono>
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

    notifications[Notification::LEVEL_UP] = DissolveTextObject{
        this, &displayBoxTitleFont, colours::titleText, "LEVEL UP"};
    notifications[Notification::SINGLE] = DissolveTextObject{
        this, &displayBoxTitleFont, colours::titleText, "SINGLE"};
    notifications[Notification::DOUBLE] = DissolveTextObject{
        this, &displayBoxTitleFont, colours::titleText, "DOUBLE"};
    notifications[Notification::TRIPLE] = DissolveTextObject{
        this, &displayBoxTitleFont, colours::titleText, "TRIPLE"};
    notifications[Notification::TETRIS] = DissolveTextObject{
        this, &displayBoxTitleFont, colours::titleText, "TETRIS"};
    notifications[Notification::MINI_TSPIN] = DissolveTextObject{
        this, &displayBoxTitleFont, colours::titleText, "MINI T-SPIN"};
    notifications[Notification::TSPIN] = DissolveTextObject{
        this, &displayBoxTitleFont, colours::titleText, "T-SPIN"};
    notifications[Notification::COMBO] = DissolveTextObject{
        this, &displayBoxTitleFont, colours::titleText, "COMBO"};
    notifications[Notification::BACKTOBACK] = DissolveTextObject{
        this, &displayBoxTitleFont, colours::titleText, "BACK-TO-BACK"};

    for (auto& [notificationType, notification] : notifications) {
        notification.recentreX(scoreText);
        notification.recentreY(grid_);
    }

    notifications[Notification::LEVEL_UP].move_y_pos(constants::levelUpYShift);

    notifications[Notification::MINI_TSPIN].move_y_pos(constants::tSpinYShift);
    notifications[Notification::TSPIN].move_y_pos(constants::tSpinYShift);

    notifications[Notification::SINGLE].move_y_pos(constants::lineClearYShift);
    notifications[Notification::DOUBLE].move_y_pos(constants::lineClearYShift);
    notifications[Notification::TRIPLE].move_y_pos(constants::lineClearYShift);
    notifications[Notification::TETRIS].move_y_pos(constants::lineClearYShift);

    notifications[Notification::COMBO].move_y_pos(constants::comboYShift);
    notifications[Notification::BACKTOBACK].move_y_pos(
        constants::backToBackYShift);

    entities_.emplace_back(std::move(heldTitle));
    entities_.emplace_back(std::move(nextTitle));
    entities_.emplace_back(std::move(scoreTitle));
    entities_.emplace_back(std::move(levelTitle));
    entities_.emplace_back(std::move(linesTitle));
}

std::string
TetrisGame::handle_input(std::chrono::milliseconds, std::chrono::milliseconds,
                         std::array<bool, utl::KeyFlag::K_TOTAL>& keyState)
{
    using namespace std::chrono_literals;
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
    if (!keyMap.at(utl::KeyFlag::K_UP) && !keyMap.at(utl::KeyFlag::K_X)
        && !keyMap.at(utl::KeyFlag::K_LCTRL) && !keyMap.at(utl::KeyFlag::K_Z)) {
        if (keyState.at(utl::KeyFlag::K_UP) || keyState.at(utl::KeyFlag::K_X)) {
            activeTetro_.rotate(flags::ROTATION::CLOCKWISE);
            lastMove_ = flags::MOVE::ROTATE;
            keyMap.at(utl::KeyFlag::K_UP) = keyState.at(utl::KeyFlag::K_UP);
            keyMap.at(utl::KeyFlag::K_X) = keyState.at(utl::KeyFlag::K_X);
            goto cleanup;
        } else if (keyState.at(utl::KeyFlag::K_LCTRL)
                   || keyState.at(utl::KeyFlag::K_Z)) {
            activeTetro_.rotate(flags::ROTATION::ANTICLOCKWISE);
            lastMove_ = flags::MOVE::ROTATE;
            keyMap.at(utl::KeyFlag::K_LCTRL) =
                keyState.at(utl::KeyFlag::K_LCTRL);
            keyMap.at(utl::KeyFlag::K_Z) = keyState.at(utl::KeyFlag::K_Z);
            goto cleanup;
        }
    } else {
        keyMap.at(utl::KeyFlag::K_UP) = keyState.at(utl::KeyFlag::K_UP);
        keyMap.at(utl::KeyFlag::K_X) = keyState.at(utl::KeyFlag::K_X);
        keyMap.at(utl::KeyFlag::K_LCTRL) = keyState.at(utl::KeyFlag::K_LCTRL);
        keyMap.at(utl::KeyFlag::K_Z) = keyState.at(utl::KeyFlag::K_Z);
    }

    // moving
    keyMap.at(utl::KeyFlag::K_LEFT) = keyState.at(utl::KeyFlag::K_LEFT);
    keyMap.at(utl::KeyFlag::K_RIGHT) = keyState.at(utl::KeyFlag::K_RIGHT);
    if (keyMap.at(utl::KeyFlag::K_LEFT) && keyMap.at(utl::KeyFlag::K_RIGHT)) {
        // both are pressed
        canMove = true;
        isMoving = false;
        isWithinFirstMove = false;
        moveTimer = 0ms;
    } else if (!keyMap.at(utl::KeyFlag::K_LEFT)
               && !keyMap.at(utl::KeyFlag::K_RIGHT)) {
        // neither is pressed
        canMove = true;
        isMoving = false;
        isWithinFirstMove = false;
        moveTimer = 0ms;
    } else {  //    exactly one is pressed
        if (canMove) {
            lastMove_ = flags::MOVE::MOVE;
            canMove = false;
            if (!isMoving)
                isWithinFirstMove = true;
            isMoving = true;

            if (keyState.at(utl::KeyFlag::K_LEFT)) {
                activeTetro_.move({-1, 0});
            }
            if (keyState.at(utl::KeyFlag::K_RIGHT)) {
                activeTetro_.move({1, 0});
            }
        }
        goto cleanup;
    }

    // soft-dropping
    if (canSoftdrop) {
        if (keyState.at(utl::KeyFlag::K_DOWN)) {
            activeTetro_.soft_drop();
            addToScore(1);
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

std::string TetrisGame::update(std::chrono::milliseconds t,
                               std::chrono::milliseconds dt)
{
    using namespace std::chrono_literals;
    if (!isRunning)
        return flags::STAGES_MAP.at(flags::STAGES::END_SCREEN);

    if (isPaused)
        return flags::STAGES_MAP.at(flags::STAGES::TETRIS);

    if (upcomingShapes_.size() < constants::shapeQueueMin)
        fillShapeQueue();

    if (isMoving) {
        moveTimer += dt;
        auto moveTime = 0ms;
        isWithinFirstMove ? moveTime = constants::moveTimerMax
                          : moveTime = constants::moveTimerMin;
        if (moveTimer >= moveTime) {
            canMove = true;
            moveTimer = 0ms;
            if (isWithinFirstMove)
                isWithinFirstMove = false;
        }
    }

    if (!canSoftdrop) {
        softdropTimer += dt;
        if (softdropTimer >= constants::softdropTimerMax) {
            canSoftdrop = true;
            softdropTimer = 0ms;
        }
    }

    timeSinceTick += dt;
    if (timeSinceTick >= tickTime_) {
        timeSinceTick = 0ms;
        activeTetro_.tick_down();
    }

    grid_.update(t, dt);
    activeTetro_.update(t, dt);
    ghostPiece.update(t, dt);
    for (const auto& entity : entities_) {
        entity->update(t, dt);
    }
    for (auto& [NotificationType, notification] : notifications)
        notification.update(t, dt);
    return flags::STAGES_MAP.at(flags::STAGES::TETRIS);
}

void TetrisGame::render(std::chrono::milliseconds, std::chrono::milliseconds)
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

    for (auto& [NotificationType, notification] : notifications)
        notification.render(renderer());

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
    if (heldDisplayBox.isLocked())
        return;

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

    heldDisplayBox.lock();
}

void TetrisGame::hardDrop()
{
    lastMove_ = flags::MOVE::HARDDROP;
    hardDropCells = ghostPiece.origin().y - activeTetro_.origin().y;
    activeTetro_.setTopLeft(ghostPiece.origin());
    grid_.notifyBlockedTetro(activeTetro_);
}

void TetrisGame::resetActiveTetro()
{
    const TetrominoShape& newShape{upcomingShapes_.front()};
    upcomingShapes_.pop();
    nextDisplayBox.updateShape(upcomingShapes_.front());
    heldDisplayBox.unlock();
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

    if (comboCount >= 1)
        notifications[Notification::COMBO].switchOn();

    int scoreThisFrame{0};

    int linePoints{determineLineClearPoints(linesCleared) * level};
    int tSpinPoints{determineTSpinPoints(linesCleared) * level};
    int comboPoints{constants::baseComboPoints * comboCount * level};
    int hardDropPoints{constants::pointsPerHardDropCell * hardDropCells};

    bool difficultClear{};
    if (linesCleared == constants::tetrisLines || tSpinPoints > 0)
        difficultClear = true;
    else
        difficultClear = false;

    tSpinPoints > 0 ? scoreThisFrame = tSpinPoints
                    : scoreThisFrame = linePoints;

    if (difficultClear && difficultClearLastTime) {
        scoreThisFrame = static_cast<int>(std::trunc(scoreThisFrame * 1.5));
        notifications[Notification::BACKTOBACK].switchOn();
    }

    if (linesCleared >= constants::singleLines)
        scoreThisFrame += comboPoints;

    scoreThisFrame += hardDropPoints;

    addToScore(scoreThisFrame);

    changeLevel();
    hardDropCells = 0;
    difficultClearLastTime = difficultClear;
}

void TetrisGame::addToScore(int points)
{
    score += points;
    scoreText.updateText(std::to_string(score));
    scoreText.recentreX(heldDisplayBox);
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

    notifications[Notification::LEVEL_UP].switchOn();
    levelText.updateText(std::to_string(level));
    levelText.recentreX(nextDisplayBox);

    using namespace std::chrono;
    using namespace std::chrono_literals;

    tickTime_ = std::chrono::milliseconds(static_cast<int>(
        std::pow(1000, 2 - level) * std::pow(807 - (7 * level), level - 1)));

    linesClearedThisLevel -= constants::linesPerLevel;
}

int TetrisGame::determineLineClearPoints(int linesCleared)
{
    switch (linesCleared) {
    case constants::singleLines:
        notifications[Notification::SINGLE].switchOn();
        return constants::singlePoints;
    case constants::doubleLines:
        notifications[Notification::DOUBLE].switchOn();
        return constants::doublePoints;
    case constants::tripleLines:
        notifications[Notification::TRIPLE].switchOn();
        return constants::triplePoints;
    case constants::tetrisLines:
        notifications[Notification::TETRIS].switchOn();
        return constants::tetrisPoints;
    default:
        return 0;
    }
}

int TetrisGame::determineTSpinPoints(int linesCleared)
{
    if (activeTetro_.shape() != T_tetromino || lastMove_ != flags::MOVE::ROTATE)
        return 0;

    flags::TSpin tSpinType{activeTetro_.checkTSpin()};
    int baseTSpinPoints{};
    int tSpinPoints{};

    switch (tSpinType) {
    case flags::TSpin::NOSPIN:
        return 0;
    case flags::TSpin::MINI:
        notifications[Notification::MINI_TSPIN].switchOn();
        baseTSpinPoints = constants::miniTSpinBasePoints;
        break;
    case flags::TSpin::TSPIN:
        notifications[Notification::TSPIN].switchOn();
        baseTSpinPoints = constants::TSpinBasePoints;
        break;
    default:
        return 0;
    }

    tSpinPoints = baseTSpinPoints * (linesCleared + 1);

    if (tSpinType == flags::TSpin::MINI
        && linesCleared == constants::doubleLines)
        tSpinPoints += constants::miniTSpinBasePoints;

    return tSpinPoints;
}
