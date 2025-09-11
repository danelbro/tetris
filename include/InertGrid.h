// -*- C++ -*-

#pragma once

#include "Cell.h"
#include "GridPoint.h"
#include "constants.h"
#include "utl_Stage.hpp"

#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>

class EndScreen;
class Grid;

class InertGrid : public utl::Entity {
public:
    InertGrid() = default;
    InertGrid(EndScreen* owner);
    InertGrid(EndScreen* owner, const Grid& grid);

    void update(double t, double dt) override final;
    void render(utl::Renderer& renderer) override final;
    const std::string& type() const override final;
    const utl::Vec2d& pos() const override final;
    const utl::Size& size() const override final;
    utl::Stage& stage() override final;
    void set_pos(const utl::Vec2d& new_pos) override;

    const Cell& get(const GridPoint& coord) const;
    void setCellColour(const GridPoint& coord, const utl::Colour& col);

public:
    const utl::Vec2d innerTopLeftPt{
        constants::gridPosX + constants::gridWallThickness,
        constants::gridPosY + constants::gridWallThickness};

private:
    void placeWalls();
    void placeBGCells();
    void enableRenderBGCells();

private:
    EndScreen* owner_{nullptr};
    std::string type_{flags::ENTITIES_MAP.at(flags::ENTITIES::GRID)};
    utl::Vec2d pos_{constants::gridPosX, constants::gridPosY};
    utl::Size size_{constants::gridWallThickness * 2
                        + constants::cellWidth * constants::gridWidth,
                    constants::gridWallThickness * 2
                        + constants::cellHeight* constants::gridHeight};

    std::vector<Cell> grid_{};
    std::array<utl::Rect, constants::gridWalls> walls_{};
};
