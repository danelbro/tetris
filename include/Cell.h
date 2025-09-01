// -*- C++ -*-
/// @file

#pragma once

#include "GridPoint.h"
#include "constants.h"

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

    void setColour(const utl::Colour& colour);
    void setOpen(bool open) { isOpen_ = open; }

    const utl::Colour& colour() const { return col; }
    const utl::Vec2d& size() const override { return size_; }

    bool renderMe() const { return renderMe_; }
    bool isOpen() const { return isOpen_; }

    void open() { isOpen_ = true; }
    void close() { isOpen_ = false; }

    const GridPoint& coord() const { return coord_; }

    // NB these functions DO NOT call set_pos() or update_rect()! You will need
    // to call them separately
    void setCoord(const GridPoint&);
    void setCoord(const int x, const int y);

private:
    utl::Rect rect;
    std::array<utl::Rect, constants::gridWalls> borders;
    utl::Colour col;
    utl::Colour borderCol;
    bool renderMe_;
    Grid& grid_;
    GridPoint coord_;
    bool isOpen_;
    utl::Vec2d size_;
};
