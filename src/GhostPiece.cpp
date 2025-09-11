#include "GhostPiece.h"

#include "Cell.h"
#include "GridPoint.h"
#include "TetrisGame.h"
#include "TetrominoShape.h"
#include "constants.h"

#include <cstdint>
#include <stdexcept>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>

static utl::Colour addTransparency(const utl::Colour& colour);

GhostPiece::GhostPiece(TetrisGame* owner)
    : utl::Entity{}, owner_{owner},
      colour_{addTransparency(owner_->activeTetro().colour())},
      origin_{owner_->activeTetro().topLeft()}, shape_{},
      currentShape_{owner_->activeTetro().shape()},
      currentRotation_{owner_->activeTetro().currentRotation()}
{
    init();
}

void GhostPiece::update(double, double)
{
    currentShape_ = owner_->activeTetro().shape();
    currentRotation_ = owner_->activeTetro().currentRotation();
    colour_ = addTransparency(owner_->activeTetro().colour());
    for (auto& cell : shape_) {
        cell.setColour(colour_);
    }
    readShape();
    repositionInGridSpace();
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

const std::string& GhostPiece::type() const
{
    return type_;
}

const utl::Vec2d& GhostPiece::pos() const
{
    return pos_;
}

const utl::Size& GhostPiece::size() const
{
    return size_;
}

utl::Stage& GhostPiece::stage()
{
    if (!owner_)
        throw std::runtime_error("GhostPiece has no owner!");
    return *owner_;
}

void GhostPiece::set_pos(const utl::Vec2d& pos)
{
    pos_ = pos;
}

const GridPoint& GhostPiece::origin() const
{
    return origin_;
}

void GhostPiece::init()
{
    shape_.reserve(constants::shapeWidth * constants::shapeHeight);
    for (size_t i{0}; i < constants::shapeWidth * constants::shapeHeight; ++i) {
        shape_.emplace_back(owner_);
    }

    readShape();
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

void GhostPiece::repositionInGridSpace()
{
    origin_.x = owner_->activeTetro().topLeft().x;

    int y{owner_->activeTetro().topLeft().y};
    for (; y <= constants::gridHeight; ++y) {
        for (const GridPoint& cell : currentShape_.at(currentRotation_)) {
            GridPoint test{origin_.x + cell.x, y + cell.y};
            if (test.x >= constants::gridWidth || test.x < 0
                || test.y >= constants::gridHeight || test.y < 0
                || !owner_->grid().get(test).isOpen()) {
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

void GhostPiece::repositionInScreenSpace()
{
    pos_.x = owner_->grid().innerTopLeftPt.x
             + static_cast<double>(origin_.x * constants::cellWidth);
    pos_.y = owner_->grid().innerTopLeftPt.y
             + static_cast<double>(origin_.y * constants::cellHeight);

    for (size_t i{0}; i < shape_.size(); ++i) {
        int x{static_cast<int>(i) % constants::shapeWidth};
        int y{static_cast<int>(i) / constants::shapeHeight};
        float newX{static_cast<float>(pos_.x)
                   + static_cast<float>(constants::cellWidth * x)};
        float newY{static_cast<float>(pos_.y)
                   + static_cast<float>(constants::cellHeight * y)};
        shape_[i].update_rect(
            {newX, newY, constants::cellWidth, constants::cellHeight});
    }
}

static utl::Colour addTransparency(const utl::Colour& colour)
{
    const uint8_t TRANSPARENCY{0x28};
    return {colour.r, colour.g, colour.b, TRANSPARENCY};
}
