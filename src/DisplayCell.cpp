#include "DisplayCell.h"

#include "DisplayBox.h"
#include "TetrisGame.h"
#include "constants.h"

#include <stdexcept>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Vec2d.hpp>

static std::array<utl::Rect, constants::gridWalls>
createBorders(utl::RectDimensions rect);
static utl::Colour shadeBorder(const utl::Colour& mainCol);

DisplayCell::DisplayCell(TetrisGame* owner, const DisplayBox& displayBox,
                         const GridPoint& coord, const utl::Colour& colour)
    : utl::Entity{}, owner_{owner},
      pos_{displayBox.pos().x + constants::displayBoxWallsThickness
               + (constants::displayCellWidth * coord.x),
           displayBox.pos().y + constants::displayBoxWallsThickness
               + (constants::displayCellWidth * coord.y)},
      col_{colour}, borderCol_{shadeBorder(colour)}, coord_{coord},
      renderMe_{true},
      rect_{{static_cast<float>(pos_.x), static_cast<float>(pos_.y),
             constants::displayCellWidth, constants::displayCellHeight}},
      borders_{createBorders({rect_.x(), rect_.y(), rect_.w(), rect_.h()})}
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

const std::string& DisplayCell::type() const
{
    return type_;
}

const utl::Vec2d& DisplayCell::pos() const
{
    return pos_;
}

const utl::Size& DisplayCell::size() const
{
    return size_;
}

utl::Stage& DisplayCell::stage()
{
    if(!owner_) throw std::runtime_error("DisplayCell has no owner!");
    return *owner_;
}

void DisplayCell::set_pos(const utl::Vec2d& newPos)
{
    pos_ = newPos;
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
    borderCol.r = static_cast<uint8_t>(borderCol.r * (1.0 - SHADE_FACTOR));
    borderCol.g = static_cast<uint8_t>(borderCol.g * (1.0 - SHADE_FACTOR));
    borderCol.b = static_cast<uint8_t>(borderCol.b * (1.0 - SHADE_FACTOR));

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
