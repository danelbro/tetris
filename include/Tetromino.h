// -*- C++ -*-
/// @file An Entity that represents a Tetromino to be drawn on the screen while
/// playing Tetris

#pragma once

#include "Cell.h"
#include "Grid.h"
#include "GridPoint.h"
#include "TetrominoShape.h"
#include "constants.h"

#include <array>
#include <utl_Entity.hpp>


class Tetromino : public utl::Entity {
public:
    Tetromino(utl::Box& screen, Grid& grid, const GridPoint& grid_point,
              const utl::Colour&, const TetrominoShape& tetrominoShape);

    void update(double t, double dt) override;
    void render(utl::Renderer& renderer) override;

    void move(int dir);
    void rotate(int dir);
    void soft_drop();

private:
    void init();
    void readShape();
    void updateShapeBoundsAndOffsets();

    void repositionInGridSpace(int x, int y);
    void repositionInScreenSpace();

    const TetrominoShape& tetrominoShape_;
    Grid& grid_;
    GridPoint topleft_;
    std::vector<Cell> shape;
    int current_width;
    int xOffset;
    int current_height;
    int yOffset;
    utl::Colour col;
    double tickTime;
    double timeSinceTick;
    size_t currentRotation;
};
