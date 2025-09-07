#include "Cell.h"

#include "Grid.h"
#include "GridPoint.h"
#include "constants.h"
#include "flags.h"

#include <array>
#include <cstdint>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>

static std::array<utl::Rect, constants::gridWalls>
createBorders(utl::RectDimensions rect);
static utl::Colour shadeBorder(const utl::Colour& mainCol);

Cell::Cell(Grid& grid, const GridPoint& coord, const utl::RectDimensions& rect,
           const utl::Colour& colour)
    : utl::Entity{}, type_{flags::ENTITIES_MAP.at(flags::ENTITIES::CELL)},
      pos_{rect.x, rect.y}, size_{rect.w, rect.h}, rect_{rect},
      borders_{createBorders(rect)}, col{colour},
      borderCol{shadeBorder(colour)}, renderMe_{false}, grid_{grid},
      coord_{coord}, isOpen_{true}
{}

const utl::Stage& Cell::stage() const
{
    return grid_.stage();
}

void Cell::update_rect(const utl::RectDimensions& rect)
{
    rect_.reset(rect);
    borders_ = createBorders(rect);
    size_ = {rect.w, rect.h};
}

void Cell::set_pos(const utl::Vec2d& newPos)
{
    pos_ = newPos;
    update_rect({static_cast<int>(pos_.x), static_cast<int>(pos_.y),
                 constants::cellWidth, constants::cellHeight});
}

void Cell::update(double, double) {}

void Cell::render(utl::Renderer& renderer)
{
    if (renderMe_) {
        utl::Colour oldCol{utl::getRendererDrawColour(renderer)};
        utl::setRendererDrawColour(renderer, col);
        rect_.draw(renderer);
        utl::setRendererDrawColour(renderer, borderCol);
        for (auto& line : borders_) line.draw(renderer);
        utl::setRendererDrawColour(renderer, oldCol);
    }
}

void Cell::setColour(const utl::Colour& colour)
{
    col = colour;
    borderCol = shadeBorder(colour);
}

void Cell::setCoord(const GridPoint& coord)
{
    if (coord.x < 0 || coord.x >= constants::gridWidth || coord.y < 0
        || coord.y >= constants::gridHeight)
        throw utl::SdlException("bad coordinate");

    coord_ = coord;
}

static utl::Colour shadeBorder(const utl::Colour& mainCol)
{
    const double SHADE_FACTOR{0.1};
    utl::Colour borderCol{mainCol};
    borderCol.r *= static_cast<uint8_t>(1.0 - SHADE_FACTOR);
    borderCol.g *= static_cast<uint8_t>(1.0 - SHADE_FACTOR);
    borderCol.b *= static_cast<uint8_t>(1.0 - SHADE_FACTOR);

    return borderCol;
}

static std::array<utl::Rect, constants::gridWalls>
createBorders(utl::RectDimensions rect)
{
    const int BORDER_WIDTH{1};

    std::array<utl::Rect, constants::gridWalls> borders;
    borders[0].reset({rect.x, rect.y, BORDER_WIDTH, rect.h});
    borders[1].reset({rect.x, rect.y, rect.w, BORDER_WIDTH});
    borders[2].reset({rect.x + rect.w, rect.y, BORDER_WIDTH, rect.h});
    borders[3].reset({rect.x, rect.y + rect.h, rect.w, BORDER_WIDTH});
    return borders;
}
