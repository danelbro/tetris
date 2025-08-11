#include "Tetromino.h"

#include "Cell.h"
#include "GridPoint.h"
#include "constants.h"
#include "flags.h"

#include <cstddef>
#include <utl_SDLInterface.hpp>
#include <utl_Vec2d.hpp>
#include <vector>

static int determineCurrentBounds(const std::vector<Cell>& shape,
                                  bool wantWidth);
static int determineOffset(const std::vector<Cell>& shape, bool wantX);

Tetromino::Tetromino(utl::Box& screen, Grid& grid, const GridPoint& grid_point,
                     const utl::Colour& colour,
                     const TetrominoShape& tetrominoShape)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::TETROMINO),
                  screen,
                  {}},
      tetrominoShape_{tetrominoShape}, grid_{grid}, topleft_{grid_point},
      shape{}, current_width{}, xOffset{}, current_height{}, yOffset{},
      col{colour}, tickTime{constants::initialTickTime}, timeSinceTick{0.0},
      xThisFrame{0}, yThisFrame{0}, maxXPerSecond{1}, maxYPerSecond{1}
{
    init();
}

void Tetromino::init()
{
    for (size_t i{0}; i < constants::shapeWidth * constants::shapeHeight; ++i) {
        shape.emplace_back(m_screenSpace, col, grid_);
    }
    updateShapeBoundsAndOffsets();
}

void Tetromino::updateShapeBoundsAndOffsets()
{
    readShape(tetrominoShape_);
    current_width = determineCurrentBounds(shape, true);
    xOffset = determineOffset(shape, true);
    current_height = determineCurrentBounds(shape, false);
    yOffset = determineOffset(shape, false);
}

void Tetromino::update(double, double dt)
{
    updateShapeBoundsAndOffsets();

    timeSinceTick += dt;
    if (timeSinceTick >= tickTime) {
        timeSinceTick = 0.0;
        yThisFrame++;
        repositionInGridSpace(xThisFrame, yThisFrame);
        xThisFrame = 0;
        yThisFrame = 0;
    }
}

void Tetromino::render(utl::Renderer& renderer)
{
    repositionInScreenSpace();

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
            if (!shape[i].renderMe())
                shape[i].makeRender();
        } else {
            if (shape[i].renderMe())
                shape[i].stopRendering();
        }
    }
}

void Tetromino::repositionInGridSpace(int x, int y)
{
    if (x > 0) {
        if (topleft_.x + xOffset + current_width + x <= constants::gridWidth) {
            topleft_.x += x;
        }
    } else if (x < 0) {
        if (topleft_.x + xOffset + x >= 0) {
            topleft_.x += x;
        }
    }

    if (y > 0) {
        if (topleft_.y + +yOffset + current_height + y
            <= constants::gridHeight) {
            topleft_.y += y;
        } else {
            grid_.notifyBottomedTetromino(*this);
        }
    } else if (y < 0) {
        if (topleft_.y + yOffset + y >= 0) {
            topleft_.y += y;
        }
    }
}

void Tetromino::repositionInScreenSpace()
{
    m_pos.x = grid_.innerTopLeftPt.x
              + static_cast<double>(topleft_.x * constants::cellWidth);
    m_pos.y = grid_.innerTopLeftPt.y
              + static_cast<double>(topleft_.y * constants::cellHeight);

    for (size_t i{0}; i < shape.size(); ++i) {
        int x{static_cast<int>(i % constants::shapeWidth)};
        int y{static_cast<int>(i / constants::shapeHeight)};
        int newX{static_cast<int>(m_pos.x) + constants::cellWidth * x};
        int newY{static_cast<int>(m_pos.y) + constants::cellHeight * y};
        // LOGF("Tetromino Cell Xpos: %d", newX);
        // LOGF("Tetromino Cell Ypos: %d\n", newY);
        shape[i].update_rect(newX, newY, constants::cellWidth,
                             constants::cellHeight);
    }
}

static int determineCurrentBounds(const std::vector<Cell>& shape,
                                  bool wantWidth)
{
    int lower{constants::shapeWidth};
    int upper{-1};

    for (size_t y{0}; y < constants::shapeHeight; ++y) {
        bool is_line_started{false};
        for (size_t x{0}; x < constants::shapeWidth; ++x) {
            bool is_cell_on{};
            if (wantWidth)
                is_cell_on = shape[x + y * constants::shapeWidth].renderMe();
            else
                is_cell_on = shape[y + x * constants::shapeWidth].renderMe();

            if (is_cell_on) {
                // NB this is not an if-else. If the line hasn't started, then
                // we want to start it and pull the lower bound here if
                // necessary. But that means the line *has now* started, and so
                // we also want to push the greater bound here too if necessary
                if (!is_line_started) {
                    is_line_started = true;
                    if (static_cast<int>(x) < lower)
                        lower = static_cast<int>(x);
                }
                if (is_line_started && static_cast<int>(x) > upper)
                    upper = static_cast<int>(x);
            } else if (is_line_started)
                is_line_started = false;
        }
    }

    int extent{upper - lower + 1};
    // #ifndef NDEBUG
    //     if (wantWidth)
    //         LOGF("current width: %d", extent);
    //     else
    //         LOGF("current height: %d", extent);
    // #endif
    return extent;
}

// determines how many empty rows there are in a shape's bounding box
// from the left (if wantX) or the top (if !wantX)
static int determineOffset(const std::vector<Cell>& shape, bool wantX)
{
    int offset{0};
    bool is_cell_on{};

    for (size_t x{0}; x < constants::shapeWidth; ++x) {
        bool maybe_offset{false};
        for (size_t y{0}; y < constants::shapeHeight; ++y) {
            if (wantX)
                is_cell_on = shape[x + y * constants::shapeWidth].renderMe();
            else
                is_cell_on = shape[y + x * constants::shapeHeight].renderMe();

            if (is_cell_on) {
                maybe_offset = false;
                break;
            }

            if (maybe_offset && y == constants::shapeHeight - 1)
                ++offset;

            maybe_offset = true;
        }
        if (is_cell_on)
            break;
    }

    // #ifndef NDEBUG
    //     if (wantX)
    //         LOGF("xOffset: %d", offset);
    //     else
    //         LOGF("yOffset: %d", offset);
    // #endif

    return offset;
}

void Tetromino::move(int dir)
{
    if (xThisFrame >= maxXPerSecond)
        return;

    xThisFrame += dir;
}

void Tetromino::rotate(int dir)
{
    LOGF("Rotating tetromino %d", dir);
}

void Tetromino::soft_drop()
{
    if (yThisFrame >= maxYPerSecond)
        return;

    yThisFrame++;
}
