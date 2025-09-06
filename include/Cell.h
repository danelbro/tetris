// -*- C++ -*-
/// @file

#pragma once

#include "GridPoint.h"
#include "constants.h"

#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>
#include <utl_Vec2d.hpp>

class Grid;

struct RectDimensions {
    int x;
    int y;
    int w;
    int h;
};

class Cell : public utl::Entity {
public:
    Cell(const Grid& grid);
    Cell(const Grid& grid, const utl::Colour& colour);
    Cell(const Grid& grid, const utl::Colour& colour, const GridPoint& coord);
    Cell(const Grid& grid, const utl::Colour& colour, const GridPoint& coord,
         const RectDimensions& rect);

    void update(double t, double dt) override;
    void render(utl::Renderer& renderer) override;
    const std::string& type() const override { return type_; }
    const utl::Vec2d& pos() const override { return pos_; }
    const utl::Vec2d& size() const override { return size_; }
    const utl::Stage& stage() const override;
    void set_pos(double x, double y) override;
    void set_pos(const utl::Vec2d& newPos) override;

    void update_rect(int x, int y, int w, int h);
    void makeRender() { renderMe_ = true; }
    void stopRendering() { renderMe_ = false; }

    void setColour(const utl::Colour& colour);
    void setOpen(bool open) { isOpen_ = open; }

    const utl::Colour& colour() const { return col; }

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
    std::string type_;
    utl::Vec2d pos_;
    utl::Vec2d size_;
    utl::Rect rect_;
    std::array<utl::Rect, constants::gridWalls> borders_;
    utl::Colour col;
    utl::Colour borderCol;
    bool renderMe_;
    const Grid& grid_;
    GridPoint coord_;
    bool isOpen_;
};
