// -*- C++ -*-
/// @file An Entity that represents a Tetromino to be drawn on the screen while
/// playing Tetris

#pragma once

#include "Cell.h"
#include "TetrominoShape.h"
#include "colours.h"
#include "flags.h"

#include <string>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>

class TetrisGame;

class Tetromino : public utl::Entity {
public:
    Tetromino() = default;
    Tetromino(TetrisGame* owner);

    void update(double t, double dt) override;
    void render(utl::Renderer& renderer) override;
    const std::string& type() const override;
    const utl::Vec2d& pos() const override;
    const utl::Size& size() const override;
    utl::Stage& stage() override;
    void set_pos(const utl::Vec2d& new_pos) override;

    void reset(const TetrominoShape& newShape);
    void move(int dir);
    void rotate(int dir);
    void tick_down();
    void soft_drop();
    void setTopLeft(const GridPoint& point);

    const utl::Colour& colour() const;

    const TetrominoShape& shape() const;
    const size_t& currentRotation() const;
    const GridPoint& topLeft() const;
    flags::TSpin checkTSpin() const;

private:
    void init();
    void readShape();

    void repositionInGridSpace(int x, int y);
    void repositionInScreenSpace();

private:
    TetrisGame* owner_{nullptr};
    std::string type_{flags::ENTITIES_MAP.at(flags::ENTITIES::TETROMINO)};
    utl::Vec2d pos_{0.0};
    utl::Size size_{};

    TetrominoShape tetrominoShape_{Z_tetromino};
    GridPoint topLeft_{0, 0};
    std::vector<Cell> shape_{};
    utl::Colour col_{colours::Z_tetrominoCol};
    size_t currentRotation_{0u};
    int dropThisTick{0};
};
