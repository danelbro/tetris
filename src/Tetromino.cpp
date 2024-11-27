#include "Tetromino.h"

#include "flags.h"

#include <utl_SDLInterface.hpp>

Tetromino::Tetromino(utl::Box& screen, utl::Colour colour)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::TETROMINO),
                  screen,
                  {screen.w / 2 - (32 / 2), 0}},
      shape{static_cast<int>(pos().x), static_cast<int>(pos().y), 32, 32},
      col{colour}, tickTime{1.0}, timeSinceTick{0.0},
      dropDistance{shape.get()->h}
{}

void Tetromino::update(double, double dt)
{
    timeSinceTick += dt;
    if (timeSinceTick >= tickTime) {
        timeSinceTick = 0.0;
        if (pos().y + dropDistance + shape.get()->h <= screen().h) {
            set_pos({pos().x, pos().y + dropDistance});
            shape.get()->y = pos().y;
        }
    }
}

void Tetromino::render(utl::Renderer& renderer)
{
    utl::Colour oldCol{utl::getRendererDrawColour(renderer)};
    utl::setRendererDrawColour(renderer, col);
    shape.draw(renderer);
    utl::setRendererDrawColour(renderer, oldCol);
}
