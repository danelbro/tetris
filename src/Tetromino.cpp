#include "Tetromino.h"

#include "Cell.h"
#include "constants.h"
#include "flags.h"

#include <cstddef>
#include <utl_SDLInterface.hpp>

Tetromino::Tetromino(utl::Box& screen, utl::Colour colour,
                     TetrominoShape tetrominoShape)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::TETROMINO),
                  screen,
                  {screen.w / 2 - (32 / 2), 0}},
      shape{}, col{colour}, tickTime{1.0}, timeSinceTick{0.0}, dropDistance{}
{
    readShape(tetrominoShape);
}

void Tetromino::update(double, double dt)
{
    timeSinceTick += dt;
    if (timeSinceTick >= tickTime) {
        timeSinceTick = 0.0;
        if (pos().y + dropDistance + constants::shapeHeight <= screen().h) {
            set_pos({pos().x, pos().y + dropDistance});
        }
    }
}

void Tetromino::render(utl::Renderer& renderer)
{
    utl::Colour oldCol{utl::getRendererDrawColour(renderer)};
    utl::setRendererDrawColour(renderer, col);
    for (auto& cell : shape) {
        cell.render(renderer);
    }
    utl::setRendererDrawColour(renderer, oldCol);
}

void Tetromino::readShape(TetrominoShape tetrominoShape)
{
    for (size_t i{0}; i < tetrominoShape.shape.size(); ++i) {
        if (tetrominoShape.shape[i]) {
            size_t x{i % constants::shapeWidth};
            size_t y{i / constants::shapeWidth};
            shape[i] =
                Cell{m_screenSpace,
                     static_cast<int>(m_pos.x + constants::cellWidth * x),
                     static_cast<int>(m_pos.y + constants::cellHeight * y),
                     constants::cellWidth,
                     constants::cellHeight,
                     col};
        }
    }
}
