// -*- C++ -*-
/// @file An Entity that represents a Tetromino to be drawn on the screen while
/// playing Tetris

#pragma once

#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>

class Tetromino : public utl::Entity {
public:
    Tetromino(utl::Box& screen, utl::Colour);

    void update(double t, double dt) override;
    void render(utl::Renderer& renderer) override;

private:
    utl::Rect shape;
    utl::Colour col;
    double tickTime;
    double timeSinceTick;
    double dropDistance;
};
