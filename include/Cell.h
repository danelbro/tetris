// -*- C++ -*-
/// @file

#pragma once

#include "GridPoint.h"
#include "colours.h"
#include "constants.h"
#include "flags.h"

#include <array>
#include <chrono>
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

    void update(std::chrono::milliseconds t,
                std::chrono::milliseconds dt) override;
    void render(utl::Renderer& renderer) override;
    const std::string& type() const override;
    const utl::Vec2d& pos() const override;
    const utl::Size& size() const override;
    utl::Stage& stage() override;
    void set_pos(const utl::Vec2d& newPos) override;

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
    utl::Vec2d pos_{};
    utl::Size size_{constants::cellWidth, constants::cellHeight};
    utl::Rect rect_{{static_cast<float>(pos_.x), static_cast<float>(pos_.y),
                     static_cast<float>(size_.w), static_cast<float>(size_.h)}};
    std::array<utl::Rect, constants::gridWalls> borders_{};
    utl::Colour col{colours::gridBG};
    utl::Colour borderCol{colours::gridBG};
    bool renderMe_{false};
    GridPoint coord_{0, 0};
    bool isOpen_{true};
};
