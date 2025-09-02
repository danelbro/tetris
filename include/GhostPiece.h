// -*- C++ -*-

#pragma once

#include "Cell.h"
#include "TetrominoShape.h"
#include "utl_Vec2d.hpp"

#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <vector>

class Grid;
class Tetromino;

class GhostPiece : public utl::Entity {
public:
    GhostPiece(utl::Box& screen, Grid& grid, const Tetromino& activeTetro);

    void update(double t, double dt) override;
    void render(utl::Renderer& renderer) override;
    const utl::Vec2d& size() const override { return size_; }

    const GridPoint& topLeft() const { return topLeft_; }

private:
    void init();
    void readShape();
    void repositionInGridSpace();
    void repositionInScreenSpace();

private:
    Grid& grid_;
    const Tetromino& activeTetro_;

    TetrominoShape currentShape_;
    size_t currentRotation_;

    utl::Colour colour_;

    std::vector<Cell> shape_;
    GridPoint topLeft_;
    utl::Vec2d size_;
};
