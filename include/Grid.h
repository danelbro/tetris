// -*- C++ -*-

#pragma once

#include "Cell.h"
#include "GridPoint.h"
#include "constants.h"

#include <array>
#include <chrono>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>
#include <utl_Vec2d.hpp>

class TetrisGame;
class Tetromino;

class Grid : public utl::Entity {
public:
    Grid() = default;
    Grid(TetrisGame* owner);
    Grid(TetrisGame* owner, const utl::Colour& colour);

    void update(std::chrono::milliseconds t,
                std::chrono::milliseconds dt) override final;
    void render(utl::Renderer& renderer) override final;
    const std::string& type() const override final;
    const utl::Vec2d& pos() const override final;
    const utl::Size& size() const override final;
    utl::Stage& stage() override final;
    void set_pos(const utl::Vec2d& new_pos) override final;

    Cell& get(const GridPoint& coord);
    const std::vector<Cell>& grid() const;

    void setCellColour(const GridPoint& coord, const utl::Colour& col);
    void setCellOpen(const GridPoint& coord, bool open);

    void notifyBlockedTetro(const Tetromino& tetromino);
    void notifyLoss(const Tetromino& tetromino);

public:
    const utl::Vec2d innerTopLeftPt{
        constants::gridPosX + constants::gridWallThickness,
        constants::gridPosY + constants::gridWallThickness};

private:
    void placeWalls();
    void placeBGCells();
    void enableRenderBGCells();
    void bakeActiveTetromino(const Tetromino& tetromino);

private:
    TetrisGame* owner_{nullptr};
    std::string type_{flags::ENTITIES_MAP.at(flags::ENTITIES::GRID)};
    utl::Vec2d pos_{constants::gridPosX, constants::gridPosY};
    utl::Size size_{(constants::gridWallThickness * 2)
                        + (constants::gridWidth * constants::cellWidth),
                    (constants::gridWallThickness * 2)
                        + (constants::gridHeight * constants::cellHeight)};

    std::array<utl::Rect, constants::gridWalls> walls_{};
    std::vector<Cell> grid_{};

    utl::Colour col{};
    int linesClearedTotal{};
    int numLinesClearedThisFrame{};
    std::vector<int> linesClearedThisFrame{};
};
