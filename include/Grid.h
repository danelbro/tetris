#pragma once

#include "Cell.h"
#include "constants.h"

#include <array>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include "utl_Vec2d.hpp"

class Tetromino;
class TetrisGame;

class Grid : public utl::Entity {
public:
    Grid(utl::Box& screen, TetrisGame& tetrisGame, const utl::Colour& colour);

    void update(double t, double dt) override;
    void render(utl::Renderer& renderer) override;

    Cell& get(unsigned x, unsigned y);

    void bakeActiveTetromino(const Tetromino& tetromino);

    const utl::Vec2d innerTopLeftPt;

private:
    void placeWalls();
    void placeBGCells();
    void enableRenderBGCells();

    TetrisGame& tetrisGame_;

    std::array<utl::Rect, constants::gridWalls> walls;
    std::vector<Cell> grid;

    utl::Colour col;
};
