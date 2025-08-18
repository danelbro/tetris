#pragma once

#include "GridPoint.h"

#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Vec2d.hpp>

class DisplayBox;

struct DisplayCell : public utl::Entity {
    DisplayCell(utl::Box& screen, const utl::Vec2d& startPos,
        const utl::Colour& colour, DisplayBox& grid, const GridPoint& coord);

    void update (double t, double dt) override;
    void render (utl::Renderer& renderer) override;

    utl::Colour col_;
    DisplayBox& grid_;
    GridPoint coord_;
    bool renderMe_;
    utl::Rect rect_;
};
