#include "Cell.h"

#include "Grid.h"
#include "GridPoint.h"
#include "constants.h"
#include "flags.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>

static utl::Box defaultBox{0, 0};
static std::array<utl::Rect, constants::gridWalls>
createBorders(float x, float y, float w, float h);
static utl::Colour shadeBorder(const utl::Colour& mainCol);

Cell::Cell(utl::Box& screen, Grid& grid)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::CELL), screen, {}},
      rect{}, borders{}, col{}, borderCol{}, renderMe_{false}, grid_{grid},
      coord_{}, isOpen_{true}, size_{}
{}

Cell::Cell(utl::Box& screen, const utl::Colour& colour, Grid& grid)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::CELL), screen, {}},
      rect{}, borders{}, col{colour}, borderCol{shadeBorder(colour)},
      renderMe_{false}, grid_{grid}, coord_{}, isOpen_{true}, size_{}
{}

Cell::Cell(utl::Box& screen, int x, int y, int w, int h,
           const utl::Colour& colour, Grid& grid, const GridPoint& coord)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::CELL),
                  screen,
                  {x, y}},
      rect{x, y, w, h},
      borders{createBorders(static_cast<float>(x), static_cast<float>(y),
                            static_cast<float>(w), static_cast<float>(h))},
      col{colour}, borderCol{shadeBorder(colour)}, renderMe_{false},
      grid_{grid}, coord_{coord}, isOpen_{true},
      size_{static_cast<double>(w), static_cast<double>(h)}
{}

Cell::Cell(utl::Box& screen, const utl::Colour& colour, Grid& grid,
           const GridPoint& coord)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::CELL), screen, {}},
      rect{}, borders{}, col{colour}, borderCol{shadeBorder(colour)},
      renderMe_{false}, grid_{grid}, coord_{coord}, isOpen_{true}, size_{}
{
    set_pos({static_cast<double>(grid_.innerTopLeftPt.x + coord_.x),
             static_cast<double>(grid_.innerTopLeftPt.y + coord_.y)});
}

Cell::Cell(const Cell& old)
    : utl::Entity{old.m_type, old.m_screenSpace, old.m_pos}, rect{}, borders{},
      col{old.col}, borderCol{old.borderCol}, renderMe_{false},
      grid_{old.grid_}, coord_{old.coord_}, isOpen_{old.isOpen_},
      size_{old.size_}
{
    if (old.rect.get()) {
        rect.reset(old.rect.x(), old.rect.y(), old.rect.w(), old.rect.h());
        for (size_t i{0}; i < old.borders.size(); ++i) {
            borders[i].reset(old.borders[i].x(), old.borders[i].y(),
                             old.borders[i].w(), old.borders[i].h());
        }
    }
}

Cell::Cell(Cell&& old) noexcept
    : utl::Entity{old.m_type, old.m_screenSpace, old.m_pos},
      rect{std::move(old.rect)}, borders{}, col{std::move(old.col)},
      borderCol{std::move(old.borderCol)}, renderMe_{old.renderMe_},
      grid_{old.grid_}, coord_{old.coord_}, isOpen_{old.isOpen_},
      size_{old.size_}
{
    for (size_t i{0}; i < old.borders.size(); ++i) {
        borders[i] = std::move(old.borders[i]);
    }
}

Cell& Cell::operator=(Cell&& old) noexcept
{
    set_pos(old.pos());
    if (old.rect.get()) {
        rect = std::move(old.rect);
        for (size_t i{0}; i < old.borders.size(); ++i) {
            borders[i] = std::move(old.borders[i]);
        }
    }
    col = std::move(old.col);
    renderMe_ = old.renderMe_;
    isOpen_ = old.isOpen_;
    size_ = old.size_;

    return *this;
}

void Cell::update_rect(int x, int y, int w, int h)
{
    rect.reset(static_cast<float>(x), static_cast<float>(y),
               static_cast<float>(w), static_cast<float>(h));
    borders = createBorders(static_cast<float>(x), static_cast<float>(y),
                            static_cast<float>(w), static_cast<float>(h));
    size_ = {static_cast<double>(w), static_cast<double>(h)};
}

void Cell::set_pos(const utl::Vec2d& newPos)
{
    m_pos = newPos;
    update_rect(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
                constants::cellWidth, constants::cellHeight);
}

void Cell::update(double, double) {}

void Cell::render(utl::Renderer& renderer)
{
    if (renderMe_) {
        utl::Colour oldCol{utl::getRendererDrawColour(renderer)};
        utl::setRendererDrawColour(renderer, col);
        rect.draw(renderer);
        utl::setRendererDrawColour(renderer, borderCol);
        for (auto& line : borders) line.draw(renderer);
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
