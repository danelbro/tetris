#include "DisplayBox.h"

#include "DisplayCell.h"
#include "TetrisGame.h"
#include "TetrominoShape.h"
#include "colours.h"
#include "constants.h"

#include <stdexcept>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Vec2d.hpp>
#include <vector>

static const utl::Colour& determineColour(const TetrominoShape& shape);
static int determineXOffset(TetrominoShape& shape);
static int determineYOffset(TetrominoShape& shape);
static void readShape(std::vector<DisplayCell>& grid, TetrominoShape& shape);

DisplayBox::DisplayBox(TetrisGame* owner) : DisplayBox{owner, {0.0, 0.0}} {}

DisplayBox::DisplayBox(TetrisGame* owner, utl::Vec2d pos)
    : utl::Entity{}, owner_{owner}, pos_{pos}
{
    populateGrid();
    placeWalls();
    updateShape(displayedShape);
    deactivate();
}

void DisplayBox::update(std::chrono::milliseconds, std::chrono::milliseconds) {}

void DisplayBox::render(utl::Renderer& renderer)
{
    utl::Colour oldCol{utl::getRendererDrawColour(renderer)};
    utl::setRendererDrawColour(renderer, colours::gridWalls);
    for (auto& wall : walls) wall.draw(renderer);
    utl::setRendererDrawColour(renderer, oldCol);

    for (auto& cell : internalGrid) cell.render(renderer);
}

const std::string& DisplayBox::type() const
{
    return type_;
}

const utl::Vec2d& DisplayBox::pos() const
{
    return pos_;
}

const utl::Size& DisplayBox::size() const
{
    return size_;
}

utl::Stage& DisplayBox::stage()
{
    if (!owner_)
        throw std::runtime_error("DisplayBox has no owner!");
    return *owner_;
}

void DisplayBox::set_pos(const utl::Vec2d& new_pos)
{
    pos_ = new_pos;
    placeWalls();
}

bool DisplayBox::isActivated()
{
    return isActive;
}

bool DisplayBox::isLocked()
{
    return isLocked_;
}

void DisplayBox::lock()
{
    isLocked_ = true;
}

void DisplayBox::unlock()
{
    isLocked_ = false;
}

void DisplayBox::activate()
{
    isActive = true;
    readShape(internalGrid, displayedShape);
}

void DisplayBox::deactivate()
{
    isActive = false;
    for (auto& cell : internalGrid) cell.setCol(colours::gridBG);
}

void DisplayBox::updateShape(const TetrominoShape& newShape)
{
    displayedShape = newShape;
    for (auto& cell : internalGrid) cell.setCol(colours::gridBG);
    if (isActive)
        readShape(internalGrid, displayedShape);
}

const TetrominoShape& DisplayBox::activeShape()
{
    return displayedShape;
}

void DisplayBox::populateGrid()
{
    for (int y{0}; y < constants::displayBoxGridHeight; ++y) {
        for (int x{0}; x < constants::displayBoxGridWidth; ++x) {
            internalGrid.emplace_back(
                DisplayCell{owner_, *this, {x, y}, colours::gridBG});
        }
    }
}

void DisplayBox::placeWalls()
{
    for (size_t i{0}; i < constants::gridWalls; ++i) {
        utl::RectDimensions rect{};
        if (i % 2 == 0) {
            // side walls
            rect.w = constants::displayBoxWallsThickness;
            rect.h = (constants::displayBoxWallsThickness * 2)
                     + (constants::displayCellHeight
                        * constants::displayBoxGridHeight);
            rect.x = static_cast<float>(pos().x)
                     + (static_cast<float>(i)
                        * (constants::displayCellWidth
                           * constants::displayBoxGridWidth)
                        / 2.0f)
                     + (static_cast<float>(i)
                        * (constants::displayBoxWallsThickness / 2.0f));
            rect.y = static_cast<float>(pos().y);
        } else {
            // top and bottom
            rect.w = (constants::displayBoxWallsThickness * 2)
                     + (constants::displayCellWidth
                        * constants::displayBoxGridWidth);
            rect.h = constants::displayBoxWallsThickness;

            rect.x = static_cast<float>(pos().x);
            rect.y = static_cast<float>(pos().y)
                     + ((static_cast<float>(i) - 1.0f)
                        * (constants::displayCellHeight
                           * (constants::displayBoxGridHeight / 2.0f)))
                     + (static_cast<float>(i) - 1)
                           * (constants::displayBoxWallsThickness / 2.0f);
        }
        walls[i] = utl::Rect{rect};
    }
}

static const utl::Colour& determineColour(const TetrominoShape& shape)
{
    switch (shape.id) {
    case 'I':
        return colours::I_tetrominoCol;
    case 'O':
        return colours::O_tetrominoCol;
    case 'T':
        return colours::T_tetrominoCol;
    case 'J':
        return colours::J_tetrominoCol;
    case 'L':
        return colours::L_tetrominoCol;
    case 'S':
        return colours::S_tetrominoCol;
    case 'Z':
        return colours::Z_tetrominoCol;
    default:  // shouldn't get here - return an "error" colour
        return colours::instructionsText;
    }
}

static int determineXOffset(TetrominoShape& shape)
{
    switch (shape.id) {
    case 'I':
    case 'O':
    case 'T':
    case 'S':
        return 1;
    case 'L':
    case 'Z':
        return 2;
    default:
        return 1;
    }
}

static int determineYOffset(TetrominoShape& shape)
{
    switch (shape.id) {
    case 'I':
        return 1;
    case 'O':
    case 'T':
    case 'J':
    case 'L':
    case 'S':
    case 'Z':
        return 2;
    default:
        return 1;
    }
}

static void readShape(std::vector<DisplayCell>& grid, TetrominoShape& shape)
{
    // we always want to display the shape in spawn orientation
    auto& current_shape{shape.at(0)};
    auto& col{determineColour(shape)};
    int xOffset{determineXOffset(shape)};
    int yOffset{determineYOffset(shape)};

    for (const auto& cell : current_shape) {
        grid[static_cast<size_t>(
                 (cell.x + xOffset)
                 + ((cell.y + yOffset) * constants::displayBoxGridWidth))]
            .setCol(col);
    }
}
