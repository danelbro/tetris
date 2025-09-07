// -*- C++ -*-
/// @file

#pragma once

#include "Grid.h"
#include "colours.h"
#include "constants.h"

#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>
#include <utl_Vec2d.hpp>

class Grid;

class Cell : public utl::Entity {
public:
    Cell(Grid& grid, const utl::Colour& colour = colours::gridBG,
         const GridPoint& coord = {0, 0},
         const utl::RectDimensions& rect = {0, 0, 0, 0});

    void update(double t, double dt) override;
    void render(utl::Renderer& renderer) override;
    const std::string& type() const override { return type_; }
    const utl::Vec2d& pos() const override { return pos_; }
    const utl::Size& size() const override { return size_; }
    const utl::Stage& stage() const override;
    void set_pos(const utl::Vec2d& newPos) override;

    void update_rect(const utl::RectDimensions& rect);
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

private:
    std::string type_;
    utl::Vec2d pos_;
    utl::Size size_;
    utl::Rect rect_;
    std::array<utl::Rect, constants::gridWalls> borders_;
    utl::Colour col;
    utl::Colour borderCol;
    bool renderMe_;
    Grid& grid_;
    GridPoint coord_;
    bool isOpen_;
};
