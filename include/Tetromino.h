// -*- C++ -*-
/// @file An Entity that represents a Tetromino to be drawn on the screen while
/// playing Tetris

#pragma once

#include "Cell.h"
#include "constants.h"

#include <array>
#include <utl_Entity.hpp>
#include <utl_Vec2d.hpp>

class Tetromino : public utl::Entity {
public:
    Tetromino(utl::Box& screen, const utl::Vec2d& pos, const utl::Colour&,
              const TetrominoShape& tetrominoShape);

    void update(double t, double dt) override;
    void render(utl::Renderer& renderer) override;

private:
    void readShape(const TetrominoShape& tetrominoShape);

    std::vector<Cell> shape;
    utl::Colour col;
    double tickTime;
    double timeSinceTick;
};
