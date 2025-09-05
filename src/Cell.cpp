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
createBorders(float x, float y, float w, float h);
static utl::Colour shadeBorder(const utl::Colour& mainCol);

Cell::Cell(Grid& grid)
    : utl::Entity{}, type_{flags::ENTITIES_MAP.at(flags::ENTITIES::CELL)},
      pos_{}, size_{}, rect_{}, borders_{}, col{}, borderCol{},
      renderMe_{false}, grid_{grid}, coord_{}, isOpen_{true}
{}

Cell::Cell(Grid& grid, const utl::Colour& colour)
    : utl::Entity{}, type_{flags::ENTITIES_MAP.at(flags::ENTITIES::CELL)},
      pos_{}, size_{}, rect_{}, borders_{}, col{colour},
      borderCol{shadeBorder(colour)}, renderMe_{false}, grid_{grid}, coord_{},
      isOpen_{true}
{}

Cell::Cell(Grid& grid, const utl::Colour& colour, const GridPoint& coord)
    : utl::Entity{}, type_{flags::ENTITIES_MAP.at(flags::ENTITIES::CELL)},
      pos_{}, size_{}, rect_{}, borders_{}, col{colour},
      borderCol{shadeBorder(colour)}, renderMe_{false}, grid_{grid},
      coord_{coord}, isOpen_{true}
{
    set_pos({static_cast<double>(grid_.innerTopLeftPt.x + coord_.x),
             static_cast<double>(grid_.innerTopLeftPt.y + coord_.y)});
}

Cell::Cell(Grid& grid, const utl::Colour& colour, const GridPoint& coord,
           const RectDimensions& rect)
    : utl::Entity{}, type_{flags::ENTITIES_MAP.at(flags::ENTITIES::CELL)},
      pos_{rect.x, rect.y},
      size_{static_cast<double>(rect.w), static_cast<double>(rect.h)},
      rect_{rect.x, rect.y, rect.w, rect.h},
      borders_{createBorders(
          static_cast<float>(rect.x), static_cast<float>(rect.y),
          static_cast<float>(rect.w), static_cast<float>(rect.h))},
      col{colour}, borderCol{shadeBorder(colour)}, renderMe_{false},
      grid_{grid}, coord_{coord}, isOpen_{true}
{}

const utl::Stage& Cell::stage() const
{
    return grid_.stage();
}

void Cell::update_rect(int x, int y, int w, int h)
{
    rect_.reset(static_cast<float>(x), static_cast<float>(y),
                static_cast<float>(w), static_cast<float>(h));
    borders_ = createBorders(static_cast<float>(x), static_cast<float>(y),
                             static_cast<float>(w), static_cast<float>(h));
    size_ = {static_cast<double>(w), static_cast<double>(h)};
}

void Cell::set_pos(const utl::Vec2d& newPos)
{
    pos_ = newPos;
    update_rect(static_cast<int>(pos_.x), static_cast<int>(pos_.y),
                constants::cellWidth, constants::cellHeight);
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

void Cell::setCoord(const int x, const int y)
{
    if (x < 0 || x >= constants::gridWidth || y < 0
        || y >= constants::gridHeight)
        throw utl::SdlException("bad coordinate");

    coord_.x = x;
    coord_.y = y;
}

void Cell::setCoord(const GridPoint& coord)
{
    setCoord(coord.x, coord.y);
}

static utl::Colour shadeBorder(const utl::Colour& mainCol)
{
    const double SHADE_FACTOR{0.1};
    utl::Colour borderCol{mainCol};
    borderCol.r = static_cast<uint8_t>(
        static_cast<int>(borderCol.r * (1.0 - SHADE_FACTOR)));
    borderCol.g = static_cast<uint8_t>(
        static_cast<int>(borderCol.g * (1.0 - SHADE_FACTOR)));
    borderCol.b = static_cast<uint8_t>(
        static_cast<int>(borderCol.b * (1.0 - SHADE_FACTOR)));

    return borderCol;
}

static std::array<utl::Rect, constants::gridWalls>
createBorders(float x, float y, float w, float h)
{
    const float BORDER_WIDTH{1.0};

    std::array<utl::Rect, constants::gridWalls> borders;
    borders[0].reset(x, y, BORDER_WIDTH, h);
    borders[1].reset(x, y, w, BORDER_WIDTH);
    borders[2].reset(x + w, y, BORDER_WIDTH, h);
    borders[3].reset(x, y + h, w, BORDER_WIDTH);
    return borders;
}
