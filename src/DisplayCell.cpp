#include "DisplayCell.h"

#include "DisplayBox.h"
#include "GridPoint.h"
#include "constants.h"
#include "flags.h"

#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Vec2d.hpp>

static std::array<utl::Rect, constants::gridWalls>
createBorders(utl::RectDimensions rect);
static utl::Colour shadeBorder(const utl::Colour& mainCol);

DisplayCell::DisplayCell(DisplayBox& grid, const GridPoint& coord,
                         const utl::Colour& colour)
    : utl::Entity{}, col_{colour}, borderCol_{shadeBorder(colour)}, grid_{grid},
      coord_{coord}, renderMe_{true},
      rect_{{static_cast<int>(grid_.pos().x), static_cast<int>(grid_.pos().y),
             constants::displayCellWidth, constants::displayCellHeight}},
      borders_{createBorders(
          {static_cast<int>(grid.pos().x), static_cast<int>(grid.pos().y),
           constants::displayCellWidth, constants::displayCellHeight})},
      type_{flags::ENTITIES_MAP.at(flags::ENTITIES::CELL)},
      pos_{grid.pos().x + constants::displayBoxWallsThickness
               + (constants::displayCellWidth * coord.x),
           grid.pos().y + constants::displayBoxWallsThickness
               + (constants::displayCellWidth * coord.y)},
      size_{constants::displayCellWidth, constants::displayCellHeight},
      owner_{grid.stage()}
{}

void DisplayCell::update(double, double) {}

void DisplayCell::render(utl::Renderer& renderer)
{
    if (renderMe_) {
        utl::Colour oldCol{utl::getRendererDrawColour(renderer)};
        utl::setRendererDrawColour(renderer, col_);
        rect_.draw(renderer);
        utl::setRendererDrawColour(renderer, borderCol_);
        for (auto& line : borders_) line.draw(renderer);
        utl::setRendererDrawColour(renderer, oldCol);
    }
}

void DisplayCell::setCol(const utl::Colour& newCol)
{
    col_ = newCol;
    borderCol_ = shadeBorder(newCol);
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
