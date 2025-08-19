#pragma once

#include "Cell.h"
#include "constants.h"

#include "utl_Vec2d.hpp"
#include <array>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>

class Tetromino;
class TetrisGame;

class Grid : public utl::Entity {
public:
    Grid(utl::Box& screen, TetrisGame& tetrisGame, const utl::Colour& colour);

    void update(double t, double dt) override;
    void render(utl::Renderer& renderer) override;

    const Cell& get(int x, int y) const;

    void notifyBlockedTetro(const Tetromino& tetromino);

    const utl::Vec2d innerTopLeftPt;
    const utl::Vec2d& size() const override { return size_; }

private:
    void placeWalls();
    void placeBGCells();
    void enableRenderBGCells();
    void bakeActiveTetromino(const Tetromino& tetromino);
    TetrisGame& tetrisGame_;

    std::array<utl::Rect, constants::gridWalls> walls;
    std::vector<Cell> grid;

    utl::Colour col;
    int linesClearedTotal;
    int numLinesClearedThisFrame;
    std::vector<int> linesClearedThisFrame;
    utl::Vec2d size_;
};
