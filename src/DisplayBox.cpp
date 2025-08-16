#include "DisplayBox.h"

#include "DisplayCell.h"
#include "TetrominoShape.h"
#include "colours.h"
#include "constants.h"
#include "flags.h"
#include "utl_SDLInterface.hpp"

#include <utl_Box.hpp>
#include <utl_Entity.hpp>

DisplayBox::DisplayBox(utl::Box& screen)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::DISPLAYBOX),
                  screen,
                  {constants::displayBoxPosX, constants::displayBoxPosY}},
      walls{}, internalGrid{}, isActive{false}, displayedShape{Z_tetromino}
{
    for (size_t y{0}; y < constants::displayBoxGridHeight; ++y) {
        for (size_t x{0}; x < constants::displayBoxGridWidth; ++x) {
            internalGrid.emplace_back(
                DisplayCell{screen,
                            colours::gridBG,
                            *this,
                            {static_cast<int>(x), static_cast<int>(y)}});
        }
    }

    placeWalls();
    updateShape(displayedShape);
    deactivate();
}

void DisplayBox::placeWalls()
{
    for (size_t i{0}; i < constants::gridWalls; ++i) {
        int w{}, h{}, x{}, y{};
        if (i % 2 == 0) {
            // side walls
            w = constants::displayBoxWallsThickness;
            h = (constants::displayBoxWallsThickness * 2)
                + (constants::displayCellHeight
                   * constants::displayBoxGridHeight);
            x = static_cast<int>(pos().x)
                + (static_cast<int>(i)
                   * (constants::displayCellWidth
                      * constants::displayBoxGridWidth)
                   / 2)
                + (static_cast<int>(i)
                   * (constants::displayBoxWallsThickness / 2));
            y = static_cast<int>(pos().y);
        } else {
            // top and bottom
            w = (constants::displayBoxWallsThickness * 2)
                + (constants::displayCellWidth
                   * constants::displayBoxGridWidth);
            h = constants::displayBoxWallsThickness;

            x = static_cast<int>(pos().x);
            y = static_cast<int>(pos().y)
                + ((static_cast<int>(i) - 1)
                   * (constants::displayCellHeight
                      * (constants::displayBoxGridHeight / 2)))
                + (static_cast<int>(i) - 1)
                      * (constants::displayBoxWallsThickness / 2);
        }
        walls[i] = utl::Rect{x, y, w, h};
    }
}

void DisplayBox::render(utl::Renderer& renderer)
{
    utl::Colour oldCol{utl::getRendererDrawColour(renderer)};
    utl::setRendererDrawColour(renderer, colours::gridWalls);
    for (auto& wall : walls) wall.draw(renderer);
    utl::setRendererDrawColour(renderer, oldCol);

    for (auto& cell : internalGrid) cell.render(renderer);
}

void DisplayBox::update(double, double) {}

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
    case 'I': case 'O': case 'T': case 'S':
        return 1;
    case 'L': case 'Z':
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
    case 'O': case 'T': case 'J': case 'L': case 'S': case 'Z':
        return 2;
    default:
        return 1;
    }
}

static void readShape(std::vector<DisplayCell>& grid, TetrominoShape& shape)
{
    // we always want to display the shape in spawn orientation
    auto& current_shape{shape.at(0)};
    auto col{determineColour(shape)};
    int xOffset{determineXOffset(shape)};
    int yOffset{determineYOffset(shape)};

    for (const auto& cell : current_shape) {
        grid[static_cast<size_t>(
                 (cell.x + xOffset)
                 + ((cell.y + yOffset) * constants::displayBoxGridWidth))]
            .col_ = col;
    }
}

void DisplayBox::updateShape(const TetrominoShape& newShape)
{
    displayedShape = newShape;
    for (auto& cell : internalGrid) cell.col_ = colours::gridBG;
    if (isActive)
        readShape(internalGrid, displayedShape);
}

void DisplayBox::activate()
{
    isActive = true;
    readShape(internalGrid, displayedShape);
}

void DisplayBox::deactivate()
{
    isActive = false;
    for (auto& cell : internalGrid) cell.col_ = colours::gridBG;
}
