#include "DisplayCell.h"

#include "GridPoint.h"
#include "constants.h"
#include "flags.h"

#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Vec2d.hpp>

DisplayCell::DisplayCell(utl::Box& screen, const utl::Vec2d& startPos,
    const utl::Colour& colour, DisplayBox& grid, const GridPoint& coord)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::CELL),
                  screen,
                  {startPos.x
                       + constants::displayBoxWallsThickness
                       + (constants::displayCellWidth * coord.x),
                   startPos.y
                       + constants::displayBoxWallsThickness
                       + (constants::displayCellWidth * coord.y)}},
      col_{colour}, grid_{grid}, coord_{coord}, renderMe_{true},
      rect_{static_cast<float>(m_pos.x), static_cast<float>(m_pos.y),
            static_cast<float>(constants::displayCellWidth),
            static_cast<float>(constants::displayCellHeight)},
      size_{constants::displayCellWidth, constants::displayCellHeight}
{}

void DisplayCell::update(double, double) {}

void DisplayCell::render(utl::Renderer& renderer)
{
    if (renderMe_) {
        utl::Colour oldCol{utl::getRendererDrawColour(renderer)};
        utl::setRendererDrawColour(renderer, col_);
        rect_.draw(renderer);
        utl::setRendererDrawColour(renderer, oldCol);
    }
}
