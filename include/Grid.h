#pragma once

#include "Cell.h"
#include "constants.h"

#include <array>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>

class Grid : public utl::Entity {
public:
    Grid(utl::Box& screen, const utl::Colour& colour);

    void update(double t, double dt) override;
    void render(utl::Renderer& renderer) override;

    Cell get(unsigned x, unsigned y);

private:
    void placeWalls();

    std::array<utl::Rect, constants::gridWalls> walls;
    std::array<Cell, constants::gridWidth * constants::gridHeight> grid;

    utl::Colour col;
};
