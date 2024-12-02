// -*- C++ -*-
/// @file

#pragma once

#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>

class Cell : public utl::Entity {
public:
    Cell(utl::Box& screen);
    Cell(utl::Box& screen, int x, int y, int w, int h,
         const utl::Colour& colour);

    Cell& operator=(Cell&&);

    void update(double t, double dt) override;
    void render(utl::Renderer& renderer) override;

private:
    utl::Rect rect;
    utl::Colour col;
};
