#include "Grid.h"

#include "Cell.h"
#include "constants.h"
#include "flags.h"

#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>

Grid::Grid(utl::Box& screen, const utl::Colour& colour)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::GRID),
                  screen,
                  {constants::gridPosX, constants::gridPosY}},
      walls{}, grid{}, col{colour}
{
    placeWalls();
}

void Grid::update(double, double) {}

void Grid::render(utl::Renderer& renderer)
{
    utl::Colour old{utl::getRendererDrawColour(renderer)};
    utl::setRendererDrawColour(renderer, col);
    for (utl::Rect& wall : walls) {
        wall.draw(renderer);
    }
    for (Cell& cell : grid) {
        cell.render(renderer);
    }
    utl::setRendererDrawColour(renderer, old);
}

Cell Grid::get(unsigned x, unsigned y)
{
    return grid[x + y * constants::gridWidth];
}

void Grid::placeWalls()
{
    for (size_t i{0}; i < constants::gridWalls; ++i) {
        int w{}, h{}, x{}, y{};
        if (i % 2 == 0) {
            // side walls
            w = constants::gridWallThickness;
            h = (constants::gridWallThickness * 2)
                + (constants::cellHeight * constants::gridHeight);
            x = pos().x
                + (i * (constants::cellWidth * constants::gridWidth) / 2)
                + (i * (constants::gridWallThickness / 2));
            y = pos().y;
        } else {
            // top and bottom
            w = (constants::gridWallThickness * 2)
                + (constants::cellWidth * constants::gridWidth);
            h = constants::gridWallThickness;
            x = pos().x;
            y = pos().y
                + ((i - 1)
                   * (constants::cellHeight * (constants::gridHeight / 2)))
                + (i - 1) * (constants::gridWallThickness / 2);
        }
        walls[i] = utl::Rect(x, y, w, h);
    }
}
