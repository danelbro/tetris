#include "Tetromino.h"

#include "Cell.h"
#include "constants.h"
#include "flags.h"

#include <cstddef>
#include <utl_SDLInterface.hpp>
#include <utl_Vec2d.hpp>

Tetromino::Tetromino(utl::Box& screen, const utl::Vec2d& pos,
                     const utl::Colour& colour,
                     const TetrominoShape& tetrominoShape)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::TETROMINO), screen,
                  pos},
      shape{}, col{colour}, tickTime{1.0}, timeSinceTick{0.0}
{
    for (size_t i{0}; i < constants::shapeWidth * constants::shapeHeight; ++i) {
        shape.emplace_back(screen, colour);
    }
    readShape(tetrominoShape);
}

void Tetromino::update(double, double dt)
{
    timeSinceTick += dt;
    if (timeSinceTick >= tickTime) {
        timeSinceTick = 0.0;
    }
}

void Tetromino::render(utl::Renderer& renderer)
{
    utl::Colour oldCol{utl::getRendererDrawColour(renderer)};
    utl::setRendererDrawColour(renderer, col);
    for (auto& cell : shape) {
        if (cell.renderMe()) {
            cell.render(renderer);
        }
    }
    utl::setRendererDrawColour(renderer, oldCol);
}

void Tetromino::readShape(const TetrominoShape& tetrominoShape)
{
    for (size_t i{0}; i < tetrominoShape.shape.size(); ++i) {
        if (tetrominoShape.shape[i]) {
            shape[i].makeRender();
            size_t x{i % constants::shapeWidth};
            size_t y{i / constants::shapeWidth};
            int newX{m_pos.x + constants::cellWidth * x};
            int newY{m_pos.x + constants::cellHeight * y};
            LOGF("Cell Xpos: %d", newX);
            LOGF("Cell Ypos: %d\n", newY);
            shape[i].update_rect(newX, newY, constants::cellWidth,
                                 constants::cellHeight);
        } else {
            shape[i].stopRendering();
        }
    }
}
