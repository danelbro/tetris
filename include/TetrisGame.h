// -*- C++ -*-
/// @file A Stage for running the main Tetris game loop.

#include "DisplayBox.h"
#include "Grid.h"
#include "Tetromino.h"
#include "TetrominoShape.h"

#include <array>
#include <memory>
#include <queue>
#include <random>
#include <string>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>
#include <vector>

class TetrisGame : public utl::Stage {
public:
    TetrisGame(utl::Box& screen, uint32_t windowID, utl::Renderer& renderer);
    std::string handle_input(double t, double dt,
                             std::array<bool, utl::KeyFlag::K_TOTAL>& keyState);
    std::string update(double t, double dt);
    void render(double t, double dt);

    void holdTetro();
    void resetActiveTetro();
    void notifyScored(int linesCleared);

private:
    const TetrominoShape& getRandomShape();
    void fillShapeQueue();

    Grid grid;
    Tetromino activeTetro;
    std::vector<std::unique_ptr<utl::Entity>> entities_;
    std::vector<TetrominoShape> possibleShapes_;
    std::queue<TetrominoShape> upcomingShapes_;
    std::mt19937 rng;
    std::uniform_int_distribution<std::mt19937::result_type> tetroDist;
    int score;

    DisplayBox displayBox;
};
