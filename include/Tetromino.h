// -*- C++ -*-
/// @file An Entity that represents a Tetromino to be drawn on the screen while
/// playing Tetris

#pragma once

#include "Cell.h"
#include "Grid.h"
#include "TetrominoShape.h"
#include "colours.h"

#include <string>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>

class Tetromino : public utl::Entity {
public:
    Tetromino(TetrisGame& owner, const GridPoint& grid_point = {0, 0},
              const TetrominoShape& tetrominoShape = Z_tetromino,
              const utl::Colour& = colours::Z_tetrominoCol);

    void update(double t, double dt) override;
    void render(utl::Renderer& renderer) override;
    const std::string& type() const override { return type_; }
    const utl::Vec2d& pos() const override { return pos_; }
    const utl::Size& size() const override { return size_; }
    const utl::Stage& stage() const override { return owner_; }

    void set_pos(const utl::Vec2d& new_pos) override { pos_ = new_pos; }

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
    const double& tickTime() const { return tickTime_; }

private:
    void init();
    void readShape();

    void repositionInGridSpace(int x, int y);
    void repositionInScreenSpace();

private:
    std::string type_;
    utl::Vec2d pos_;
    utl::Size size_;
    TetrisGame& owner_;

    TetrominoShape tetrominoShape_;
    GridPoint topLeft_;
    std::vector<Cell> shape_;
    utl::Colour col_;
    double tickTime_;
    double timeSinceTick;
    size_t currentRotation_;
};
