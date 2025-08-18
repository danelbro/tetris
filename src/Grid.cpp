#include "Grid.h"

#include "Cell.h"
#include "TetrisGame.h"
#include "Tetromino.h"
#include "colours.h"
#include "constants.h"
#include "flags.h"

#include <stdexcept>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Vec2d.hpp>

static int countLinesToClear(const std::vector<Cell>& grid,
                             std::vector<int>& clearedLines);
static void clearLines(std::vector<Cell>& grid,
                       const std::vector<int>& clearedLines);
static void applyGravity(std::vector<Cell>& grid,
                         const std::vector<int>& clearedLines);

Grid::Grid(utl::Box& screen, TetrisGame& tetrisGame, const utl::Colour& colour)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::GRID),
                  screen,
                  {constants::gridPosX, constants::gridPosY}},
      innerTopLeftPt{constants::gridPosX + constants::gridWallThickness,
                     constants::gridPosY + constants::gridWallThickness},
      tetrisGame_{tetrisGame}, walls{}, grid{}, col{colour},
      linesClearedTotal{0}, numLinesClearedThisFrame{0}, linesClearedThisFrame{}
{
    linesClearedThisFrame.reserve(static_cast<size_t>(constants::gridHeight));
    for (size_t i{0}; i < constants::gridWidth * constants::gridHeight; ++i) {
        grid.emplace_back(screen, *this);
    }
    placeWalls();
    placeBGCells();
    enableRenderBGCells();
}

void Grid::update(double, double)
{
    numLinesClearedThisFrame = 0;
    linesClearedThisFrame.clear();
}

void Grid::render(utl::Renderer& renderer)
{
    utl::Colour old{utl::getRendererDrawColour(renderer)};
    utl::setRendererDrawColour(renderer, col);
    for (utl::Rect& wall : walls) {
        wall.draw(renderer);
    }
    for (Cell& cell : grid) {
        cell.render(renderer);
    }
    utl::setRendererDrawColour(renderer, old);
}

const Cell& Grid::get(int x, int y) const
{
    if (x < 0 || y < 0)
        throw std::runtime_error("overflow");

    return grid[static_cast<size_t>(x + y * constants::gridWidth)];
}

void Grid::placeWalls()
{
    for (size_t i{0}; i < constants::gridWalls; ++i) {
        int w{}, h{}, x{}, y{};
        if (i % 2 == 0) {
            // side walls
            w = constants::gridWallThickness;
            h = (constants::gridWallThickness * 2)
                + (constants::cellHeight * constants::gridHeight);
            x = static_cast<int>(pos().x)
                + (static_cast<int>(i)
                   * (constants::cellWidth * constants::gridWidth) / 2)
                + (static_cast<int>(i) * (constants::gridWallThickness / 2));
            y = static_cast<int>(pos().y);
        } else {
            // top and bottom
            w = (constants::gridWallThickness * 2)
                + (constants::cellWidth * constants::gridWidth);
            h = constants::gridWallThickness;

            x = static_cast<int>(pos().x);
            y = static_cast<int>(pos().y)
                + ((static_cast<int>(i) - 1)
                   * (constants::cellHeight * (constants::gridHeight / 2)))
                + (static_cast<int>(i) - 1)
                      * (constants::gridWallThickness / 2);
        }
        walls[i] = utl::Rect{x, y, w, h};
    }
}

void Grid::placeBGCells()
{
    const utl::Vec2d base_pos{pos().x + constants::gridWallThickness,
                              pos().y + constants::gridWallThickness};

    for (size_t y{0}; y < constants::gridHeight; ++y) {
        for (size_t x{0}; x < constants::gridWidth; ++x) {
            grid[x + y * constants::gridWidth].set_pos(
                {base_pos.x + static_cast<double>(x * constants::cellWidth),
                 base_pos.y + static_cast<double>(y * constants::cellHeight)});
        }
    }
}

void Grid::enableRenderBGCells()
{
    for (size_t i{0}; i < grid.size(); ++i) {
        grid[i].setColour(colours::gridBG);
        grid[i].makeRender();
    }
}

void Grid::notifyBlockedTetro(const Tetromino& tetromino)
{
    bakeActiveTetromino(tetromino);
    tetrisGame_.resetActiveTetro();

    numLinesClearedThisFrame = countLinesToClear(grid, linesClearedThisFrame);

    if (numLinesClearedThisFrame > 0) {
        clearLines(grid, linesClearedThisFrame);
        applyGravity(grid, linesClearedThisFrame);
        tetrisGame_.notifyScored(numLinesClearedThisFrame);
    }
}

void Grid::bakeActiveTetromino(const Tetromino& tetromino)
{
    const auto& newColour{tetromino.colour()};

    for (const GridPoint& cell :
         tetromino.shape().at(tetromino.currentRotation())) {
        Cell& gridCell{grid[static_cast<size_t>(
            (cell.x + tetromino.topLeft().x)
            + (cell.y + tetromino.topLeft().y) * constants::gridWidth)]};
        gridCell.setColour(newColour);
        gridCell.close();
    }
}

static void clearLines(std::vector<Cell>& grid,
                       const std::vector<int>& clearedLines)
{
    for (const auto& line : clearedLines) {
        for (int x{0}; x < constants::gridWidth; ++x) {
            Cell& activeCell{
                grid[static_cast<size_t>(x + line * constants::gridWidth)]};
            activeCell.open();
            activeCell.setColour(colours::gridBG);
        }
    }
}

static int countLinesToClear(const std::vector<Cell>& grid,
                             std::vector<int>& clearedLines)
{
    clearedLines.clear();
    int filledCells{0};
    int numOfClearedLines{0};

    for (int y{0}; y < constants::gridHeight; ++y) {
        filledCells = 0;
        for (int x{0}; x < constants::gridWidth; ++x) {
            if (!grid[static_cast<size_t>(x + y * constants::gridWidth)]
                     .isOpen())
                ++filledCells;
        }

        if (filledCells == constants::gridWidth) {
            clearedLines.push_back(y);
            ++numOfClearedLines;
        }
    }

    return numOfClearedLines;
}

static void applyGravity(std::vector<Cell>& grid,
                         const std::vector<int>& linesCleared)
{
    for (int line : linesCleared) {
        for (int y{line - 1}; y >= 0; y--) {
            for (int x{0}; x < constants::gridWidth; ++x) {
                Cell& activeCell{
                    grid[static_cast<size_t>(x + y * constants::gridWidth)]};

                if (!activeCell.isOpen()) {
                    utl::Colour transferColour{activeCell.colour()};
                    activeCell.open();
                    activeCell.setColour(colours::gridBG);

                    Cell& lowerCell{grid[static_cast<size_t>(
                        x + (y + 1) * constants::gridWidth)]};
                    lowerCell.close();
                    lowerCell.setColour(transferColour);
                }
            }
        }
    }
}
