#include "Cell.h"

#include "Grid.h"
#include "GridPoint.h"
#include "constants.h"
#include "flags.h"

#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>

static utl::Box defaultBox{0, 0};

Cell::Cell(utl::Box& screen, Grid& grid)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::CELL), screen, {}},
      rect{}, col{}, renderMe_{false}, grid_{grid}, coord_{}
{}

Cell::Cell(utl::Box& screen, const utl::Colour& colour, Grid& grid)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::CELL), screen, {}},
      rect{}, col{colour}, renderMe_{false}, grid_{grid}, coord_{}
{}

Cell::Cell(utl::Box& screen, int x, int y, int w, int h,
           const utl::Colour& colour, Grid& grid, const GridPoint& coord)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::CELL),
                  screen,
                  {x, y}},
      rect{x, y, w, h}, col{colour}, renderMe_{false}, grid_{grid},
      coord_{coord}
{}

Cell::Cell(utl::Box& screen, const utl::Colour& colour, Grid& grid,
           const GridPoint& coord)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::CELL), screen, {}},
      rect{}, col{colour}, renderMe_{false}, grid_{grid}, coord_{coord}
{
    set_pos({static_cast<double>(grid_.innerTopLeftPt.x + coord_.x),
             static_cast<double>(grid_.innerTopLeftPt.y + coord_.y)});
}

Cell::Cell(const Cell& old)
    : utl::Entity{old.m_type, old.m_screenSpace, old.m_pos}, rect{},
      col{old.col}, renderMe_{false}, grid_{old.grid_}, coord_{old.coord_}
{
    if (old.rect.get()) {
        rect.reset(old.rect.x(), old.rect.y(), old.rect.w(), old.rect.h());
    }
}

Cell::Cell(Cell&& old) noexcept
    : utl::Entity{old.m_type, old.m_screenSpace, old.m_pos},
      rect{std::move(old.rect)}, col{std::move(old.col)},
      renderMe_{old.renderMe_}, grid_{old.grid_}, coord_{old.coord_}
{}

Cell& Cell::operator=(Cell&& old) noexcept
{
    set_pos(old.pos());
    if (old.rect.get()) {
        rect.reset(old.rect.x(), old.rect.y(), old.rect.w(), old.rect.h());
    }
    col = std::move(old.col);
    renderMe_ = old.renderMe_;

    return *this;
}

void Cell::update_rect(int x, int y, int w, int h)
{
    rect.reset(static_cast<float>(x), static_cast<float>(y),
               static_cast<float>(w), static_cast<float>(h));
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
        utl::setRendererDrawColour(renderer, oldCol);
    }
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
