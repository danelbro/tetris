#pragma once

#include "Cell.h"
#include "TetrisGame.h"
#include "constants.h"

#include "utl_Vec2d.hpp"
#include <array>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>

class Tetromino;

class Grid : public utl::Entity {
public:
    Grid(TetrisGame& owner);
    Grid(TetrisGame& owner, const utl::Colour& colour);

    void update(double t, double dt) override;
    void render(utl::Renderer& renderer) override;
    const std::string& type() const override { return type_; }
    const utl::Vec2d& pos() const override { return pos_; }
    const utl::Vec2d& size() const override { return size_; }
    const utl::Stage& stage() const override { return owner_; };
    void set_pos(double x, double y) override;
    void set_pos(const utl::Vec2d& new_pos) override;

    const Cell& get(int x, int y) const;

    void setCellColour(int x, int y, utl::Colour col);
    void setCellOpen(int x, int y, bool open);

    void notifyBlockedTetro(const Tetromino& tetromino);
    void notifyLoss(const Tetromino& tetromino);

public:
    const utl::Vec2d innerTopLeftPt;

private:
    void placeWalls();
    void placeBGCells();
    void enableRenderBGCells();
    void bakeActiveTetromino(const Tetromino& tetromino);

private:
    std::string type_;
    utl::Vec2d pos_;
    utl::Vec2d size_;
    TetrisGame& owner_;

    std::array<utl::Rect, constants::gridWalls> walls;
    std::vector<Cell> grid;

    utl::Colour col;
    int linesClearedTotal;
    int numLinesClearedThisFrame;
    std::vector<int> linesClearedThisFrame;
};
