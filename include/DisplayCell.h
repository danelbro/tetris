#pragma once

#include "GridPoint.h"
#include "constants.h"

#include <array>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Vec2d.hpp>

class DisplayBox;

struct DisplayCell : public utl::Entity {
    DisplayCell(utl::Box& screen, const utl::Vec2d& startPos,
                const utl::Colour& colour, DisplayBox& grid,
                const GridPoint& coord);

    void update(double t, double dt) override;
    void render(utl::Renderer& renderer) override;
    const utl::Vec2d& size() const override { return size_; };

    void setCol(const utl::Colour& newCol);

    utl::Colour col_;
    utl::Colour borderCol_;
    DisplayBox& grid_;
    GridPoint coord_;
    bool renderMe_;
    utl::Rect rect_;
    std::array<utl::Rect, constants::gridWalls> borders_;
    utl::Vec2d size_;
};
