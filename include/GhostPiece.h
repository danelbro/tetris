// -*- C++ -*-

#pragma once

#include "Cell.h"
#include "TetrisGame.h"
#include "TetrominoShape.h"
#include "utl_Vec2d.hpp"

#include <string>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <vector>

class Grid;
class Tetromino;

class GhostPiece : public utl::Entity {
public:
    GhostPiece(TetrisGame& owner);

    void update(double t, double dt) override;
    void render(utl::Renderer& renderer) override;
    const std::string& type() const override { return type_; }
    const utl::Vec2d& pos() const override { return pos_; }
    const utl::Vec2d& size() const override { return size_; }
    const utl::Stage& stage() const override { return owner_; }

    const GridPoint& origin() const { return origin_; }

private:
    void init();
    void readShape();
    void repositionInGridSpace();
    void repositionInScreenSpace();

private:
    std::string type_;
    utl::Vec2d pos_;
    utl::Vec2d size_;
    TetrisGame& owner_;

    utl::Colour colour_;

    GridPoint origin_;
    std::vector<Cell> shape_;
    TetrominoShape currentShape_;
    size_t currentRotation_;
};
