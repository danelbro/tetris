// -*- C++ -*-
/// @file

#pragma once

#include <GridPoint.h>

#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Vec2d.hpp>

class Grid;

class Cell : public utl::Entity {
public:
    Cell(utl::Box& screen, Grid& grid);
    Cell(utl::Box& screen, const utl::Colour& colour, Grid& grid);
    Cell(utl::Box& screen, int x, int y, int w, int h,
         const utl::Colour& colour, Grid& grid, const GridPoint& coord);
    Cell(utl::Box& screen, const utl::Colour& colour, Grid& grid,
         const GridPoint& coord);

    Cell(const Cell&);
    Cell(Cell&&) noexcept;
    Cell& operator=(const Cell&) = delete;
    Cell& operator=(Cell&&) noexcept;

    void update_rect(int x, int y, int w, int h);
    void set_pos(const utl::Vec2d& newPos);

    void update(double t, double dt) override;
    void render(utl::Renderer& renderer) override;

    void makeRender() { renderMe_ = true; }
    void stopRendering() { renderMe_ = false; }

    void setColour(const utl::Colour& colour) { col = colour; }

    const utl::Colour& colour() const { return col; }

    bool renderMe() const { return renderMe_; }

    const GridPoint& coord() const { return coord_; }

    // NB these functions DO NOT cal set_pos() or update_rect()! You will need
    // to call them separately
    void setCoord(const GridPoint&);
    void setCoord(const int x, const int y);

private:
    utl::Rect rect;
    utl::Colour col;
    bool renderMe_;
    Grid& grid_;
    GridPoint coord_;
};
