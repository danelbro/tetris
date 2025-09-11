#include "Cell.h"

#include "constants.h"

#include <array>
#include <cstdint>
#include <stdexcept>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>

static std::array<utl::Rect, constants::gridWalls>
createBorders(utl::RectDimensions rect);
static utl::Colour shadeBorder(const utl::Colour& mainCol);

Cell::Cell(utl::Stage* owner) : Cell{owner, {}, {}, {}} {}

Cell::Cell(utl::Stage* owner, const utl::Colour& colour, const GridPoint& coord,
           const utl::RectDimensions& rect)
    : utl::Entity{}, stage_{owner}, rect_{rect}, pos_{rect.x, rect.y},
      size_{rect.w, rect.h}, borders_{createBorders(rect)}, col{colour},
      borderCol{shadeBorder(colour)}, renderMe_{false}, coord_{coord},
      isOpen_{true}
{}

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

const std::string& Cell::type() const
{
    return type_;
}

const utl::Vec2d& Cell::pos() const
{
    return pos_;
}

const utl::Size& Cell::size() const
{
    return size_;
}

utl::Stage& Cell::stage()
{
    if (!stage_)
        throw std::runtime_error("Cell has no owning stage!");
    return *stage_;
}

void Cell::set_pos(const utl::Vec2d& newPos)
{
    pos_ = newPos;
    update_rect({static_cast<float>(pos_.x), static_cast<float>(pos_.y),
                 constants::cellWidth, constants::cellHeight});
}

void Cell::update_rect(const utl::RectDimensions& rect)
{
    rect_.reset(rect);
    borders_ = createBorders(rect);
    size_ = {static_cast<double>(rect.w), static_cast<double>(rect.h)};
}

bool Cell::renderMe() const
{
    return renderMe_;
}

void Cell::makeRender()
{
    renderMe_ = true;
}

void Cell::stopRendering()
{
    renderMe_ = false;
}

const utl::Colour& Cell::colour() const
{
    return col;
}

void Cell::setColour(const utl::Colour& colour)
{
    col = colour;
    borderCol = shadeBorder(colour);
}

bool Cell::isOpen() const
{
    return isOpen_;
}

void Cell::setOpen(bool open)
{
    isOpen_ = open;
}

void Cell::open()
{
    isOpen_ = true;
}

void Cell::close()
{
    isOpen_ = false;
}

const GridPoint& Cell::coord() const
{
    return coord_;
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
