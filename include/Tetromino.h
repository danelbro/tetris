// -*- C++ -*-
/// @file An Entity that represents a Tetromino to be drawn on the screen while
/// playing Tetris

#pragma once

#include "Cell.h"
#include "Grid.h"
#include "GridPoint.h"
#include "constants.h"

#include <utl_Entity.hpp>
#include <utl_Vec2d.hpp>

class Tetromino : public utl::Entity {
public:
    Tetromino(utl::Box& screen, const Grid& grid, const GridPoint& grid_point,
              const utl::Colour&, const TetrominoShape& tetrominoShape);

    void update(double t, double dt) override;
    void render(utl::Renderer& renderer) override;

private:
    void readShape(const TetrominoShape& tetrominoShape);

    void repositionInGridSpace(int x, int y);
    void repositionInScreenSpace();

    const TetrominoShape& tetrominoShape_;
    const Grid& grid_;
    GridPoint topleft_;
    std::vector<Cell> shape;
    int current_width;
    int current_height;
    utl::Colour col;
    double tickTime;
    double timeSinceTick;
};
