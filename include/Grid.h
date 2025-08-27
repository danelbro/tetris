#pragma once

#include "Cell.h"
#include "constants.h"

#include "utl_Vec2d.hpp"
#include <array>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_Stage.hpp>
#include <utl_SDLInterface.hpp>

class Tetromino;

class Grid : public utl::Entity {
public:
    Grid(utl::Box& screen, utl::Stage& owner, const utl::Colour& colour);

    void update(double t, double dt) override;
    void render(utl::Renderer& renderer) override;

    const Cell& get(int x, int y) const;

    void setCellColour (int x, int y, utl::Colour col);
    void setCellOpen (int x, int y, bool open);

    void notifyBlockedTetro(const Tetromino& tetromino);
    void notifyLoss(const Tetromino& tetromino);

    const utl::Vec2d innerTopLeftPt;
    const utl::Vec2d& size() const override { return size_; }

private:
    void placeWalls();
    void placeBGCells();
    void enableRenderBGCells();
    void bakeActiveTetromino(const Tetromino& tetromino);
    utl::Stage& owner_;

    std::array<utl::Rect, constants::gridWalls> walls;
    std::vector<Cell> grid;

    utl::Colour col;
    int linesClearedTotal;
    int numLinesClearedThisFrame;
    std::vector<int> linesClearedThisFrame;
    utl::Vec2d size_;
};
