#include "Grid.h"

#include "Cell.h"
#include "TetrisGame.h"
#include "Tetromino.h"
#include "colours.h"
#include "constants.h"

#include <stdexcept>
#include <utl_Box.hpp>
#include <utl_Entity.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>
#include <utl_Vec2d.hpp>

static int countLinesToClear(const std::vector<Cell>& grid,
                             std::vector<int>& clearedLines);
static void clearLines(std::vector<Cell>& grid,
                       const std::vector<int>& clearedLines);
static void applyGravity(std::vector<Cell>& grid,
                         const std::vector<int>& clearedLines);

Grid::Grid(TetrisGame* owner) : Grid{owner, colours::gridBG} {}

Grid::Grid(TetrisGame* owner, const utl::Colour& colour)
    : utl::Entity{}, owner_{owner}, col{colour}
{
    linesClearedThisFrame.reserve(static_cast<size_t>(constants::gridHeight));
    for (size_t i{0}; i < constants::gridWidth * constants::gridHeight; ++i) {
        grid_.emplace_back(owner_);
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
    for (utl::Rect& wall : walls_) {
        wall.draw(renderer);
    }
    for (Cell& cell : grid_) {
        cell.render(renderer);
    }
    utl::setRendererDrawColour(renderer, old);
}

const std::string& Grid::type() const
{
    return type_;
}

const utl::Vec2d& Grid::pos() const
{
    return pos_;
}

const utl::Size& Grid::size() const
{
    return size_;
}

utl::Stage& Grid::stage()
{
    if (!owner_)
        throw std::runtime_error("Grid has no owner!");
    return *owner_;
}

void Grid::set_pos(const utl::Vec2d& new_pos)
{
    pos_ = new_pos;
    placeWalls();
}

const Cell& Grid::get(const GridPoint& coord) const
{
    if (coord.x < 0 || coord.y < 0)
        throw std::runtime_error("overflow");

    return grid_[static_cast<size_t>(coord.x + coord.y * constants::gridWidth)];
}

const std::vector<Cell>& Grid::grid() const
{
    return grid_;
}

void Grid::setCellColour(const GridPoint& coord, const utl::Colour& colour)
{
    grid_[static_cast<size_t>(coord.x + coord.y * constants::gridWidth)]
        .setColour(colour);
}

void Grid::setCellOpen(const GridPoint& coord, bool open)
{
    grid_[static_cast<size_t>(coord.x + coord.y * constants::gridWidth)]
        .setOpen(open);
}

void Grid::notifyBlockedTetro(const Tetromino& tetromino)
{
    bakeActiveTetromino(tetromino);
    owner_->resetActiveTetro();

    numLinesClearedThisFrame = countLinesToClear(grid_, linesClearedThisFrame);

    if (numLinesClearedThisFrame > 0) {
        clearLines(grid_, linesClearedThisFrame);
        applyGravity(grid_, linesClearedThisFrame);
        owner_->notifyScored(numLinesClearedThisFrame);
    }
}

void Grid::notifyLoss(const Tetromino& tetromino)
{
    bakeActiveTetromino(tetromino);
    owner_->notifyLoss();
}

void Grid::placeWalls()
{
    for (size_t i{0}; i < constants::gridWalls; ++i) {
        utl::RectDimensions rect{};
        if (i % 2 == 0) {
            // side walls
            rect.w = constants::gridWallThickness;
            rect.h = (constants::gridWallThickness * 2)
                + (constants::cellHeight * constants::gridHeight);
            rect.x = static_cast<float>(pos().x)
                + (static_cast<float>(i)
                   * (constants::cellWidth * constants::gridWidth) / 2.0f)
                + (static_cast<float>(i) * (constants::gridWallThickness / 2.0f));
            rect.y = static_cast<float>(pos().y);
        } else {
            // top and bottom
            rect.w = (constants::gridWallThickness * 2.0f)
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

void Grid::placeBGCells()
{
    const utl::Vec2d base_pos{pos().x + constants::gridWallThickness,
                              pos().y + constants::gridWallThickness};

    for (size_t y{0}; y < constants::gridHeight; ++y) {
        for (size_t x{0}; x < constants::gridWidth; ++x) {
            grid_[x + y * constants::gridWidth].set_pos(
                {base_pos.x + static_cast<double>(x * constants::cellWidth),
                 base_pos.y + static_cast<double>(y * constants::cellHeight)});
        }
    }
}

void Grid::bakeActiveTetromino(const Tetromino& tetromino)
{
    const auto& newColour{tetromino.colour()};

    for (const GridPoint& cell :
         tetromino.shape().at(tetromino.currentRotation())) {
        Cell& gridCell{grid_[static_cast<size_t>(
            (cell.x + tetromino.topLeft().x)
            + (cell.y + tetromino.topLeft().y) * constants::gridWidth)]};
        gridCell.setColour(newColour);
        gridCell.close();
    }
}

void Grid::enableRenderBGCells()
{
    for (auto& cell : grid_) {
        cell.setColour(colours::gridBG);
        cell.makeRender();
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

