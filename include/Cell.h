// -*- C++ -*-
/// @file

#pragma once

#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Vec2d.hpp>

class Cell : public utl::Entity {
public:
    Cell(utl::Box& screen);
    Cell(utl::Box& screen, const utl::Colour& colour);
    Cell(utl::Box& screen, int x, int y, int w, int h,
         const utl::Colour& colour);
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

private:
    utl::Rect rect;
    utl::Colour col;
    bool renderMe_;
};
