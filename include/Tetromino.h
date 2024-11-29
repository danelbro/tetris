// -*- C++ -*-
/// @file An Entity that represents a Tetromino to be drawn on the screen while
/// playing Tetris

#pragma once

#include "Cell.h"
#include "constants.h"

#include <array>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>

struct TetrominoShape {
    std::array<int, constants::shapeWidth * constants::shapeHeight> shape;
};

class Tetromino : public utl::Entity {
public:
    Tetromino(utl::Box& screen, utl::Colour, TetrominoShape tetrominoShape);

    void update(double t, double dt) override;
    void render(utl::Renderer& renderer) override;

private:
    void readShape(TetrominoShape tetrominoShape);

    std::array<Cell, constants::shapeWidth * constants::shapeHeight> shape;
    utl::Colour col;
    double tickTime;
    double timeSinceTick;
    double dropDistance;
};
