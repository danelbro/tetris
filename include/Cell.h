// -*- C++ -*-
/// @file

#pragma once

#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>

class Cell : public utl::Entity {
public:
    Cell();
    Cell(utl::Box& screen);

    void update(double t, double dt) override;
    void render(utl::Renderer& renderer) override;
};
