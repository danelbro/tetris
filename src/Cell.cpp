#include "Cell.h"

#include "flags.h"

#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>

static utl::Box defaultBox{0, 0};

Cell::Cell()
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::CELL), defaultBox, {}}
{}

Cell::Cell(utl::Box& screen)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::CELL), screen, {}}
{}

void Cell::update(double, double) {}

void Cell::render(utl::Renderer&) {}
