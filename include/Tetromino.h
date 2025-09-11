// -*- C++ -*-
/// @file An Entity that represents a Tetromino to be drawn on the screen while
/// playing Tetris

#pragma once

#include "Cell.h"
#include "TetrominoShape.h"
#include "colours.h"

#include <string>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>

class TetrisGame;

class Tetromino : public utl::Entity {
public:
    Tetromino(TetrisGame* owner, const GridPoint& grid_point = {0, 0},
              const TetrominoShape& tetrominoShape = Z_tetromino,
              const utl::Colour& = colours::Z_tetrominoCol);

    void update(double t, double dt) override final;
    void render(utl::Renderer& renderer) override final;
    const std::string& type() const override final;
    const utl::Vec2d& pos() const override final;
    const utl::Size& size() const override final;
    utl::Stage& stage() override final;
    void set_pos(const utl::Vec2d& new_pos) override final;

    void reset(const TetrominoShape& newShape);
    void move(int dir);
    void rotate(int dir);
    void soft_drop();
    void changeTickTime(double newTickTime);
    void setTopLeft(const GridPoint& point);

    const utl::Colour& colour() const;

    const TetrominoShape& shape() const;
    const size_t& currentRotation() const;
    const GridPoint& topLeft() const;
    const double& tickTime() const;

private:
    void init();
    void readShape();

    void repositionInGridSpace(int x, int y);
    void repositionInScreenSpace();

private:
    std::string type_;
    utl::Vec2d pos_;
    utl::Size size_;
    TetrisGame* owner_;

    TetrominoShape tetrominoShape_;
    GridPoint topLeft_;
    std::vector<Cell> shape_;
    utl::Colour col_;
    double tickTime_;
    double timeSinceTick;
    size_t currentRotation_;
};
