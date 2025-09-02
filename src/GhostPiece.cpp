#include "GhostPiece.h"

#include "Cell.h"
#include "Grid.h"
#include "GridPoint.h"
#include "Tetromino.h"
#include "constants.h"
#include "flags.h"

#include <cstdint>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>

static utl::Colour addTransparency(const utl::Colour& colour);

GhostPiece::GhostPiece(utl::Box& screen, Grid& grid,
                       const Tetromino& activeTetro)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::GHOST_PIECE),
                  screen,
                  {}},
      grid_{grid}, activeTetro_{activeTetro},
      currentShape_{activeTetro_.shape()},
      currentRotation_{activeTetro_.currentRotation()},
      colour_{addTransparency(activeTetro_.colour())}, shape_{},
      topLeft_{activeTetro_.topLeft()}, size_{}
{
    init();
}

void GhostPiece::init()
{
    shape_.reserve(constants::shapeWidth * constants::shapeHeight);
    for (size_t i{0}; i < constants::shapeWidth * constants::shapeHeight; ++i) {
        shape_.emplace_back(m_screenSpace, colour_, grid_);
    }

    readShape();
}

void GhostPiece::repositionInScreenSpace()
{
    m_pos.x = grid_.innerTopLeftPt.x
              + static_cast<double>(topLeft_.x * constants::cellWidth);
    m_pos.y = grid_.innerTopLeftPt.y
              + static_cast<double>(topLeft_.y * constants::cellHeight);

    for (size_t i{0}; i < shape_.size(); ++i) {
        int x{static_cast<int>(i % constants::shapeWidth)};
        int y{static_cast<int>(i / constants::shapeHeight)};
        int newX{static_cast<int>(m_pos.x) + constants::cellWidth * x};
        int newY{static_cast<int>(m_pos.y) + constants::cellHeight * y};
        shape_[i].update_rect(newX, newY, constants::cellWidth,
                              constants::cellHeight);
    }
}

void GhostPiece::repositionInGridSpace()
{
    topLeft_.x = activeTetro_.topLeft().x;

    int y{activeTetro_.topLeft().y};
    for (; y <= constants::gridHeight; ++y) {
        for (const GridPoint& cell : currentShape_.at(currentRotation_)) {
            int testX{topLeft_.x + cell.x};
            int testY{y + cell.y};
            if (testX >= constants::gridWidth || testX < 0
                || testY >= constants::gridHeight || testY < 0
                || !grid_.get(testX, testY).isOpen()) {
                // shape is blocked here
                goto shapeBlocked;
            }
        }
        // shape is open here
        continue;
    }
    // we are at maximum depth
shapeBlocked:
    topLeft_.y = y - 1; //  we want the shape to be in the last open space we found
}

void GhostPiece::update(double, double)
{
    currentShape_ = activeTetro_.shape();
    currentRotation_ = activeTetro_.currentRotation();
    colour_ = addTransparency(activeTetro_.colour());
    for (auto& cell : shape_) {
        cell.setColour(colour_);
    }
    readShape();
    repositionInGridSpace();
}

void GhostPiece::readShape()
{
    for (auto& cell : shape_) cell.stopRendering();

    auto& current_shape{currentShape_.at(currentRotation_)};

    for (const auto& cell : current_shape) {
        shape_[static_cast<size_t>(cell.x + cell.y * constants::shapeWidth)]
            .makeRender();
    }
}

void GhostPiece::render(utl::Renderer& renderer)
{
    repositionInScreenSpace();

    utl::Colour oldCol{utl::getRendererDrawColour(renderer)};
    utl::setRendererDrawColour(renderer, colour_);
    for (auto& cell : shape_) {
        if (cell.renderMe()) {
            cell.render(renderer);
        }
    }
    utl::setRendererDrawColour(renderer, oldCol);
}

static utl::Colour addTransparency(const utl::Colour& colour)
{
    const uint8_t TRANSPARENCY{0x28};
    return {colour.r, colour.g, colour.b, TRANSPARENCY};
}
