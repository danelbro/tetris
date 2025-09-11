// -*- C++ -*-
/// @file

#pragma once

#include "GridPoint.h"
#include "constants.h"
#include "flags.h"

#include <array>
#include <string>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>
#include <utl_Vec2d.hpp>

class Cell : public utl::Entity {
public:
    Cell() = default;
    Cell(utl::Stage* stage);
    Cell(utl::Stage* stage, const utl::Colour& colour, const GridPoint& coord,
         const utl::RectDimensions& rect);

    void update(double t, double dt) override final;
    void render(utl::Renderer& renderer) override final;
    const std::string& type() const override final;
    const utl::Vec2d& pos() const override final;
    const utl::Size& size() const override final;
    utl::Stage& stage() override final;
    void set_pos(const utl::Vec2d& newPos) override final;

    void update_rect(const utl::RectDimensions& rect);

    bool renderMe() const;
    void makeRender();
    void stopRendering();

    const utl::Colour& colour() const;
    void setColour(const utl::Colour& colour);

    bool isOpen() const;
    void setOpen(bool open);
    void open();
    void close();

    const GridPoint& coord() const;

    // NB this function DO NOT call set_pos() or update_rect()! You will need
    // to call them separately
    void setCoord(const GridPoint&);

private:
    utl::Stage* stage_{nullptr};
    std::string type_{flags::ENTITIES_MAP.at(flags::ENTITIES::CELL)};
    utl::Rect rect_{};
    utl::Vec2d pos_{};
    utl::Size size_{};
    std::array<utl::Rect, constants::gridWalls> borders_{};
    utl::Colour col{};
    utl::Colour borderCol{};
    bool renderMe_{false};
    GridPoint coord_{};
    bool isOpen_{true};
};
