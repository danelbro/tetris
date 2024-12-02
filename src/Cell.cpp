#include "Cell.h"

#include "flags.h"

#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>

static utl::Box defaultBox{0, 0};

Cell::Cell(utl::Box& screen)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::CELL), screen, {}}
{}

Cell::Cell(utl::Box& screen, int x, int y, int w, int h,
           const utl::Colour& colour)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::CELL),
                  screen,
                  {x, y}},
      rect{x, y, w, h}, col{colour}
{}

Cell& Cell::operator=(Cell&& old)
{
    set_pos(old.pos());
    rect.get()->x = old.rect.get()->x;
    rect.get()->y = old.rect.get()->y;
    rect.get()->w = old.rect.get()->w;
    rect.get()->h = old.rect.get()->h;
    col = old.col;

    return *this;
}

void Cell::update(double, double) {}

void Cell::render(utl::Renderer&) {}
