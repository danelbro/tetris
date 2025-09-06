// -*- C++ -*-
/// @file A Stage for running the main Tetris game loop.
#pragma once

#include "DisplayBox.h"
#include "GhostPiece.h"
#include "Grid.h"
#include "Tetromino.h"
#include "TetrominoShape.h"

#include <array>
#include <memory>
#include <queue>
#include <random>
#include <string>
#include <unordered_map>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>
#include <utl_TextObject.hpp>
#include <utl_random.hpp>
#include <vector>

class TetrisGame : public utl::Stage {
public:
    TetrisGame(utl::Box& screen, uint32_t windowID, utl::Renderer& renderer);
    std::string handle_input(double t, double dt,
                             std::array<bool, utl::KeyFlag::K_TOTAL>& keyState);
    std::string update(double t, double dt);
    void render(double t, double dt);

    void holdTetro();
    void hardDrop();
    void resetActiveTetro();
    void notifyScored(int linesCleared);
    void notifyLoss();

    const Grid& grid() const { return grid_; }
    const Tetromino& activeTetro() const { return activeTetro(); }
    int getScore() const { return score; }
    int getLines() const { return linesClearedTotal; }
    int getLevel() const { return level; }

private:
    const TetrominoShape& getRandomShape();
    void fillShapeQueue();
    void changeLevel();

    Grid grid_;
    Tetromino activeTetro;
    GhostPiece ghostPiece;

    std::vector<std::unique_ptr<utl::Entity>> entities_;
    std::vector<TetrominoShape> possibleShapes_;
    std::queue<TetrominoShape> upcomingShapes_;
    utl::RNG rng;
    std::uniform_int_distribution<std::mt19937::result_type> tetroDist;
    int score;
    int linesClearedTotal;
    int linesClearedThisLevel;
    int level;

    utl::Font displayBoxTitleFont;

    DisplayBox heldDisplayBox;
    DisplayBox nextDisplayBox;

    std::unordered_map<utl::KeyFlag, bool> keyMap;
    bool canRotate;
    double rotateTimer;
    bool canMove;
    double moveTimer;
    bool canSoftdrop;
    double softdropTimer;
    bool canHarddrop;

    bool running;

    utl::TextObject scoreText;
    utl::TextObject levelText;
    utl::TextObject linesText;
};
