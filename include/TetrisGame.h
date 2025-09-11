// -*- C++ -*-
/// @file A Stage for running the main Tetris game loop.

#pragma once

#include "DisplayBox.h"
#include "GhostPiece.h"
#include "Grid.h"
#include "Tetromino.h"
#include "TetrominoShape.h"
#include "flags.h"

#include <array>
#include <memory>
#include <queue>
#include <random>
#include <string>
#include <unordered_map>
#include <utl_Application.hpp>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>
#include <utl_TextObject.hpp>
#include <utl_random.hpp>
#include <vector>

class TetrisGame : public utl::Stage {
public:
    TetrisGame(utl::Application& tetris_app);

    std::string handle_input(
        double t, double dt,
        std::array<bool, utl::KeyFlag::K_TOTAL>& keyState) override final;
    std::string update(double t, double dt) override final;
    void render(double t, double dt) override final;
    utl::Application& app() override final;
    utl::Box& screen() override final;
    utl::Renderer& renderer() override final;

    void holdTetro();
    void hardDrop();
    void resetActiveTetro();
    void notifyScored(int linesCleared);
    void notifyLoss();

    Grid& grid();
    const Tetromino& activeTetro() const;
    int getScore() const;
    int getLines() const;
    int getLevel() const;

private:
    const TetrominoShape& getRandomShape();
    void fillShapeQueue();
    void changeLevel();

private:
    std::string label{flags::STAGES_MAP.at(flags::STAGES::TETRIS)};
    utl::Application& app_;
    Grid grid_{this, colours::gridWalls};
    Tetromino activeTetro_{this};
    GhostPiece ghostPiece{this};

    std::vector<std::unique_ptr<utl::Entity>> entities_{};
    std::vector<TetrominoShape> possibleShapes_{};
    std::queue<TetrominoShape> upcomingShapes_{};
    utl::RNG rng{};
    std::uniform_int_distribution<std::mt19937::result_type> tetroDist{};
    int score{0};
    int linesClearedTotal{0};
    int linesClearedThisLevel{0};
    int level{1};

    utl::Font displayBoxTitleFont;

    DisplayBox heldDisplayBox{
        this, {constants::heldDisplayBoxPosX, constants::heldDisplayBoxPosY}};
    DisplayBox nextDisplayBox{
        this, {constants::nextDisplayBoxPosX, constants::nextDisplayBoxPosY}};

    std::unordered_map<utl::KeyFlag, bool> keyMap{};
    bool canRotate{true};
    double rotateTimer{0.0};
    bool canMove{true};
    double moveTimer{0.0};
    bool canSoftdrop{true};
    double softdropTimer{0.0};
    bool canHarddrop{true};

    bool running{true};

    utl::TextObject scoreText{};
    utl::TextObject levelText{};
    utl::TextObject linesText{};
};
