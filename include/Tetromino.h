// -*- C++ -*-
/// @file An Entity that represents a Tetromino to be drawn on the screen while
/// playing Tetris

#pragma once

#include "Cell.h"
#include "Grid.h"
#include "GridPoint.h"
#include "TetrominoShape.h"

#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>

class Tetromino : public utl::Entity {
public:
    Tetromino(utl::Box& screen, Grid& grid, const GridPoint& grid_point,
              const utl::Colour&, const TetrominoShape& tetrominoShape);

    void update(double t, double dt) override;
    void render(utl::Renderer& renderer) override;

    void reset(const TetrominoShape& newShape);

    void move(int dir);
    void rotate(int dir);
    void soft_drop();
    void changeTickTime(double newTickTime);
    void setTopLeft(const GridPoint& point) { topLeft_ = point; }

    const utl::Colour& colour() const { return col_; }

    const TetrominoShape& shape() const { return tetrominoShape_; }
    const size_t& currentRotation() const { return currentRotation_; }
    const GridPoint& topLeft() const { return topLeft_; }
    const utl::Vec2d& size() const override { return size_; }
    const double& tickTime() const { return tickTime_; }

private:
    void init();
    void readShape();

    void repositionInGridSpace(int x, int y);
    void repositionInScreenSpace();

    TetrominoShape tetrominoShape_;
    Grid& grid_;
    GridPoint topLeft_;
    std::vector<Cell> shape_;
    utl::Colour col_;
    double tickTime_;
    double timeSinceTick;
    size_t currentRotation_;
    utl::Vec2d size_;
};
