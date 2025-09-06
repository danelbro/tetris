#include "GhostPiece.h"

#include "Cell.h"
#include "Grid.h"
#include "GridPoint.h"
#include "TetrisGame.h"
#include "constants.h"
#include "flags.h"

#include <cstdint>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>

static utl::Colour addTransparency(const utl::Colour& colour);

GhostPiece::GhostPiece(TetrisGame& owner)
    : utl::Entity{},
      type_{flags::ENTITIES_MAP.at(flags::ENTITIES::GHOST_PIECE)}, pos_{},
      size_{}, owner_{owner},
      colour_{addTransparency(owner.activeTetro().colour())},
      origin_{owner.activeTetro().topLeft()}, shape_{},
      currentShape_{owner.activeTetro().shape()},
      currentRotation_{owner.activeTetro().currentRotation()}
{
    init();
}

void GhostPiece::init()
{
    shape_.reserve(constants::shapeWidth * constants::shapeHeight);
    for (size_t i{0}; i < constants::shapeWidth * constants::shapeHeight; ++i) {
        shape_.emplace_back(owner_.grid(), colour_);
    }

    readShape();
}

void GhostPiece::repositionInScreenSpace()
{
    pos_.x = owner_.grid().innerTopLeftPt.x
             + static_cast<double>(origin_.x * constants::cellWidth);
    pos_.y = owner_.grid().innerTopLeftPt.y
             + static_cast<double>(origin_.y * constants::cellHeight);

    for (size_t i{0}; i < shape_.size(); ++i) {
        int x{static_cast<int>(i % constants::shapeWidth)};
        int y{static_cast<int>(i / constants::shapeHeight)};
        int newX{static_cast<int>(pos_.x) + constants::cellWidth * x};
        int newY{static_cast<int>(pos_.y) + constants::cellHeight * y};
        shape_[i].update_rect(newX, newY, constants::cellWidth,
                              constants::cellHeight);
    }
}

void GhostPiece::repositionInGridSpace()
{
    origin_.x = owner_.activeTetro().topLeft().x;

    int y{owner_.activeTetro().topLeft().y};
    for (; y <= constants::gridHeight; ++y) {
        for (const GridPoint& cell : currentShape_.at(currentRotation_)) {
            int testX{origin_.x + cell.x};
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
    origin_.y =
        y - 1;  //  we want the shape to be in the last open space we found
}

void GhostPiece::update(double, double)
{
    currentShape_ = owner_.activeTetro().shape();
    currentRotation_ = owner_.activeTetro().currentRotation();
    colour_ = addTransparency(owner_.activeTetro().colour());
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
