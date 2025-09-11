#include "InertGrid.h"

#include "EndScreen.h"
#include "Grid.h"
#include "colours.h"
#include "constants.h"

#include <stdexcept>
#include <utl_SDLInterface.hpp>

InertGrid::InertGrid(EndScreen* owner) : owner_{owner}
{
    for (int i{0}; i < constants::gridWidth * constants::gridHeight; ++i)
        grid_.emplace_back(owner);

    placeWalls();
    placeBGCells();
    enableRenderBGCells();
}

InertGrid::InertGrid(EndScreen* owner, const Grid& grid) : owner_{owner}
{
    for (int i{0}; i < constants::gridWidth * constants::gridHeight; ++i)
        grid_.emplace_back(owner);

    placeWalls();
    placeBGCells();
    enableRenderBGCells();

    for (int y{0}; y < constants::gridHeight; ++y) {
        for (int x{0}; x < constants::gridWidth; ++x) {
            setCellColour({x, y}, grid.get({x, y}).colour());
        }
    }
}

void InertGrid::update(double, double) {}

void InertGrid::render(utl::Renderer& renderer)
{
    utl::Colour old{utl::getRendererDrawColour(renderer)};
    utl::setRendererDrawColour(renderer, colours::gridWalls);
    for (utl::Rect& wall : walls_) {
        wall.draw(renderer);
    }
    for (Cell& cell : grid_) {
        cell.render(renderer);
    }
    utl::setRendererDrawColour(renderer, old);
}

const std::string& InertGrid::type() const
{
    return type_;
}

const utl::Vec2d& InertGrid::pos() const
{
    return pos_;
}

const utl::Size& InertGrid::size() const
{
    return size_;
}

utl::Stage& InertGrid::stage()
{
    if (!owner_)
        throw std::runtime_error("InertGrid has no owner!");
    return *owner_;
}

void InertGrid::set_pos(const utl::Vec2d& new_pos)
{
    pos_ = new_pos;
    placeWalls();
}

const Cell& InertGrid::get(const GridPoint& coord) const
{
    if (coord.x < 0 || coord.y < 0)
        throw std::runtime_error("overflow");

    return grid_[static_cast<size_t>(coord.x + coord.y * constants::gridWidth)];
}

void InertGrid::setCellColour(const GridPoint& coord, const utl::Colour& col)
{
    grid_[static_cast<size_t>(coord.x + coord.y * constants::gridWidth)]
        .setColour(col);
}

void InertGrid::placeWalls()
{
    for (size_t i{0}; i < constants::gridWalls; ++i) {
        utl::RectDimensions rect{};
        if (i % 2 == 0) {
            // side walls
            rect.w = constants::gridWallThickness;
            rect.h = (constants::gridWallThickness * 2)
                     + (constants::cellHeight * constants::gridHeight);
            rect.x =
                static_cast<float>(pos().x)
                + (static_cast<float>(i)
                   * (constants::cellWidth * constants::gridWidth) / 2.0f)
                + (static_cast<float>(i) * (constants::gridWallThickness / 2.0f));
            rect.y = static_cast<float>(pos().y);
        } else {
            // top and bottom
            rect.w = (constants::gridWallThickness * 2)
                     + (constants::cellWidth * constants::gridWidth);
            rect.h = constants::gridWallThickness;

            rect.x = static_cast<float>(pos().x);
            rect.y = static_cast<float>(pos().y)
                     + ((static_cast<float>(i) - 1.0f)
                        * (constants::cellHeight * (constants::gridHeight / 2.0f)))
                     + (static_cast<float>(i) - 1.0f)
                           * (constants::gridWallThickness / 2.0f);
        }
        walls_[i] = utl::Rect{rect};
    }
}

void InertGrid::placeBGCells()
{
    for (size_t y{0}; y < constants::gridHeight; ++y) {
        for (size_t x{0}; x < constants::gridWidth; ++x) {
            Cell& activeCell{grid_[x + y * constants::gridWidth]};
            activeCell.setCoord({static_cast<int>(x), static_cast<int>(y)});
            activeCell.set_pos(
                {innerTopLeftPt.x
                     + static_cast<double>(x * constants::cellWidth),
                 innerTopLeftPt.y
                     + static_cast<double>(y * constants::cellHeight)});
            activeCell.update_rect({static_cast<float>(activeCell.pos().x),
                                    static_cast<float>(activeCell.pos().y),
                                    constants::cellWidth,
                                    constants::cellHeight});
        }
    }
}

void InertGrid::enableRenderBGCells()
{
    for (size_t i{0}; i < grid_.size(); ++i) {
        grid_[i].setColour(colours::gridBG);
        grid_[i].makeRender();
    }
}
