// -*- C++ -*-

#pragma once

#include "Cell.h"
#include "GridPoint.h"
#include "TetrominoShape.h"
#include "colours.h"

#include <chrono>
#include <string>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Vec2d.hpp>
#include <vector>

class TetrisGame;

class GhostPiece : public utl::Entity {
public:
    GhostPiece(TetrisGame* owner);

    void update(std::chrono::milliseconds t,
                std::chrono::milliseconds dt) override;
    void render(utl::Renderer& renderer) override;
    const std::string& type() const override;
    const utl::Vec2d& pos() const override;
    const utl::Size& size() const override;
    utl::Stage& stage() override;
    void set_pos(const utl::Vec2d& pos) override;

    const GridPoint& origin() const;

private:
    void init();
    void readShape();
    void repositionInGridSpace();
    void repositionInScreenSpace();

private:
    TetrisGame* owner_{nullptr};
    std::string type_{flags::ENTITIES_MAP.at(flags::ENTITIES::GHOST_PIECE)};
    utl::Vec2d pos_{};
    utl::Size size_{};

    utl::Colour colour_{colours::Z_tetrominoCol};

    GridPoint origin_{};
    std::vector<Cell> shape_{};
    TetrominoShape currentShape_{Z_tetromino};
    size_t currentRotation_{};
};
