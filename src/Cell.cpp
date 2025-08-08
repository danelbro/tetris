#include "Cell.h"

#include "constants.h"
#include "flags.h"

#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>

static utl::Box defaultBox{0, 0};

Cell::Cell(utl::Box& screen)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::CELL), screen, {}},
      rect{}, col{}, renderMe_{false}
{}

Cell::Cell(utl::Box& screen, const utl::Colour& colour)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::CELL), screen, {}},
      rect{}, col{colour}, renderMe_{false}
{}

Cell::Cell(utl::Box& screen, int x, int y, int w, int h,
           const utl::Colour& colour)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::CELL),
                  screen,
                  {x, y}},
      rect{x, y, w, h}, col{colour}, renderMe_{false}
{}

Cell::Cell(const Cell& old)
    : utl::Entity{old.m_type, old.m_screenSpace, old.m_pos}, rect{},
      col{old.col}, renderMe_{false}
{
    if (old.rect.get()) {
        rect.reset(old.rect.x(), old.rect.y(), old.rect.w(),
                   old.rect.h());
    }
}

Cell::Cell(Cell&& old)
    : utl::Entity{std::move(old.m_type), old.m_screenSpace,
                  std::move(old.m_pos)},
      rect{std::move(old.rect)}, col{std::move(old.col)},
      renderMe_{old.renderMe_}
{}

Cell& Cell::operator=(Cell&& old)
{
    set_pos(std::move(old.pos()));
    if (old.rect.get()) {
        rect.reset(std::move(old.rect.x()), std::move(old.rect.y()),
                   std::move(old.rect.w()), std::move(old.rect.h()));
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
