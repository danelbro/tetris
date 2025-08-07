#include "Tetromino.h"

#include "Cell.h"
#include "GridPoint.h"
#include "constants.h"
#include "flags.h"

#include <cstddef>
#include <utl_SDLInterface.hpp>
#include <utl_Vec2d.hpp>
#include <vector>

static int determineCurrentWidth(const std::vector<Cell>& shape);
static int determineCurrentHeight(const std::vector<Cell>& shape);

Tetromino::Tetromino(utl::Box& screen, const Grid& grid,
                     const GridPoint& grid_point, const utl::Colour& colour,
                     const TetrominoShape& tetrominoShape)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::TETROMINO),
                  screen,
                  {}},
      tetrominoShape_{tetrominoShape}, grid_{grid}, topleft_{grid_point},
      shape{}, current_width{constants::shapeWidth},
      current_height{constants::shapeHeight}, col{colour},
      tickTime{constants::initialTickTime}, timeSinceTick{0.0}
{
    for (size_t i{0}; i < constants::shapeWidth * constants::shapeHeight; ++i) {
        shape.emplace_back(screen, colour);
    }
    readShape(tetrominoShape_);
}

void Tetromino::update(double, double dt)
{
    readShape(tetrominoShape_);
    current_width = determineCurrentWidth(shape);
    current_height = determineCurrentHeight(shape);
    timeSinceTick += dt;
    if (timeSinceTick >= tickTime) {
        timeSinceTick = 0.0;
        repositionInGridSpace(-1, -1);
    }
    repositionInScreenSpace();
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
        if (topleft_.x + current_width + x <= constants::gridWidth) {
            topleft_.x += x;
        }
    } else if (x < 0) {
        if (topleft_.x + x >= 0) {
            topleft_.x += x;
        }
    }

    if (y > 0) {
        if (topleft_.y + current_height + y <= constants::gridHeight) {
            topleft_.y += y;
        }
    } else if (y < 0) {
        if (topleft_.y + y >= 0) {
            topleft_.y += y;
        }
    }
}

void Tetromino::repositionInScreenSpace()
{
    m_pos.x = grid_.innerTopLeft.x + (topleft_.x * constants::cellWidth);
    m_pos.y = grid_.innerTopLeft.y + (topleft_.y * constants::cellHeight);

    for (size_t i{0}; i < shape.size(); ++i) {
        size_t x{i % constants::shapeWidth};
        size_t y{i / constants::shapeHeight};
        int newX{static_cast<int>(m_pos.x + constants::cellWidth * x)};
        int newY{static_cast<int>(m_pos.y + constants::cellHeight * y)};
        // LOGF("Tetromino Cell Xpos: %d", newX);
        // LOGF("Tetromino Cell Ypos: %d\n", newY);
        shape[i].update_rect(newX, newY, constants::cellWidth,
                             constants::cellHeight);
    }
}

static int determineCurrentWidth(const std::vector<Cell>& shape)
{
    int max_width{-1};
    int running_width{0};
    bool is_line_started{false};
    bool is_cell_on{false};

    for (size_t y{0}; y < constants::shapeHeight; ++y) {
        running_width = 0;
        is_line_started = false;
        for (size_t x{0}; x < constants::shapeWidth; ++x) {
            is_cell_on = shape[x + y * constants::shapeWidth].renderMe();
            if (!is_line_started) {
                if (is_cell_on) {
                    is_line_started = true;
                    ++running_width;
                    continue;
                } else {
                    continue;
                }
            } else {
                if (is_cell_on) {
                    ++running_width;
                    continue;
                } else {
                    is_line_started = false;
                    if (running_width > max_width) {
                        max_width = running_width;
                    }
                    continue;
                }
            }
        }
        if (running_width > max_width) {
            max_width = running_width;
        }
    }

    return max_width;
}

static int determineCurrentHeight(const std::vector<Cell>& shape)
{
    int max_height{-1};
    int running_height{0};
    bool is_line_started{false};
    bool is_cell_on{false};

    for (size_t x{0}; x < constants::shapeWidth; ++x) {
        running_height = 0;
        is_line_started = false;
        for (size_t y{0}; y < constants::shapeHeight; ++y) {
            is_cell_on = shape[x + y * constants::shapeWidth].renderMe();
            if (!is_line_started) {
                if (is_cell_on) {
                    is_line_started = true;
                    ++running_height;
                    continue;
                } else {
                    continue;
                }
            } else {
                if (is_cell_on) {
                    ++running_height;
                    continue;
                } else {
                    is_line_started = false;
                    if (running_height > max_height) {
                        max_height = running_height;
                    }
                    continue;
                }
            }
        }
        if (running_height > max_height) {
            max_height = running_height;
        }
    }

    return max_height;
}
