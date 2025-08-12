#include "Tetromino.h"

#include "Cell.h"
#include "GridPoint.h"
#include "TetrominoShape.h"
#include "colours.h"
#include "constants.h"
#include "flags.h"

#include <cstddef>
#include <utl_SDLInterface.hpp>
#include <utl_Vec2d.hpp>
#include <vector>

static const utl::Colour& determineColour(const TetrominoShape&);

Tetromino::Tetromino(utl::Box& screen, Grid& grid, const GridPoint& grid_point,
                     const utl::Colour& colour,
                     const TetrominoShape& tetrominoShape)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::TETROMINO),
                  screen,
                  {}},
      tetrominoShape_{tetrominoShape}, grid_{grid}, topLeft_{grid_point},
      shape_{}, col_{colour},
      tickTime{constants::initialTickTime}, timeSinceTick{0.0},
      rotationTimer{0.0}, rotationLengthInSecs{0.09},
      moveTimer{0.0}, moveLengthInSecs{0.08},
      dropTimer{0.0}, dropLengthInSecs{0.05},
      isRotating{false}, isMoving{false}, isDropping{false},
      currentRotation_{0}
{
    init();
}

void Tetromino::init()
{
    for (size_t i{0}; i < constants::shapeWidth * constants::shapeHeight; ++i) {
        shape_.emplace_back(m_screenSpace, col_, grid_);
    }

    readShape();
}

void Tetromino::update(double, double dt)
{
    readShape();

    if (isRotating) {
        if (rotationTimer < rotationLengthInSecs)
            rotationTimer += dt;
        else {
            rotationTimer = 0.0;
            isRotating = false;
        }
    }

    if (isMoving) {
        if (moveTimer < moveLengthInSecs)
            moveTimer += dt;
        else {
            moveTimer = 0.0;
            isMoving = false;
        }
    }

    if (isDropping) {
        if (dropTimer < dropLengthInSecs)
            dropTimer += dt;
        else {
            dropTimer = 0.0;
            isDropping = false;
        }
    }

    timeSinceTick += dt;
    if (timeSinceTick >= tickTime) {
        timeSinceTick = 0.0;
        repositionInGridSpace(0, 1);
    }
}

void Tetromino::render(utl::Renderer& renderer)
{
    repositionInScreenSpace();

    utl::Colour oldCol{utl::getRendererDrawColour(renderer)};
    utl::setRendererDrawColour(renderer, col_);
    for (auto& cell : shape_) {
        if (cell.renderMe()) {
            cell.render(renderer);
        }
    }
    utl::setRendererDrawColour(renderer, oldCol);
}

void Tetromino::readShape()
{
    for (auto& cell : shape_)
        cell.stopRendering();

    auto& current_shape{
        tetrominoShape_.at(static_cast<size_t>(currentRotation_))
    };

    for (const auto& cell : current_shape) {
        shape_[
            static_cast<size_t>(
                cell.x + cell.y * constants::shapeWidth)].makeRender();
    }
}

void Tetromino::repositionInGridSpace(int x, int y)
{
    // we start by assuming that the tetromino can move anywhere, and if any of
    // the conditions are broken we change that
    bool canMoveRight{ true }, canMoveLeft{ true },
        canMoveDown{ true }, canMoveUp{ true };

    if (x > 0) {
        for (const GridPoint& cell : tetrominoShape_.at(currentRotation_)) {
            if (topLeft_.x + cell.x + x >= constants::gridWidth ||
                !grid_.get(static_cast<size_t>(topLeft_.x + cell.x + x),
                    static_cast<size_t>(topLeft_.y + cell.y)).isOpen())
                canMoveRight = false;
        }
        if (canMoveRight) topLeft_.x += x;
    } else if (x < 0) {
        for (const GridPoint& cell : tetrominoShape_.at(currentRotation_)) {
            if (topLeft_.x + cell.x + x < 0 ||
                !grid_.get(static_cast<size_t>(topLeft_.x + cell.x + x),
                    static_cast<size_t>(topLeft_.y + cell.y)).isOpen())
                canMoveLeft = false;
        }
        if (canMoveLeft) topLeft_.x += x;
    }

    if (y > 0) {
        for (const GridPoint& cell : tetrominoShape_.at(currentRotation_)) {
            if (topLeft_.y + cell.y + y >= constants::gridHeight ||
                !grid_.get(static_cast<size_t>(topLeft_.x + cell.x),
                    static_cast<size_t>(topLeft_.y + cell.y + y)).isOpen())
                canMoveDown = false;
        }
        if (canMoveDown) topLeft_.y += y;
        else grid_.bakeActiveTetromino(*this);
    } else if (y < 0) {    // just for completeness
        for (const GridPoint& cell : tetrominoShape_.at(currentRotation_)) {
            if (topLeft_.y + cell.y + y < 0 ||
                !grid_.get(static_cast<size_t>(topLeft_.x + cell.x),
                    static_cast<size_t>(topLeft_.y + cell.y + y)).isOpen())
                canMoveUp = false;
        }
        if (canMoveUp) topLeft_.y += y;
    }
}

void Tetromino::repositionInScreenSpace()
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


void Tetromino::move(int dir)
{
    if (isMoving) return;

    repositionInGridSpace(dir, 0);

    isMoving = true;
}

void Tetromino::rotate(int dir)
{
    if (isRotating) return;

    size_t new_rotation{ (currentRotation_ + dir + constants::rotations)
        % constants::rotations };

    currentRotation_ = new_rotation;

    isRotating = true;
}

void Tetromino::soft_drop()
{
    if (isDropping) return;

    repositionInGridSpace(0, 1);

    isDropping = true;
}

void Tetromino::reset(const TetrominoShape& newShape)
{
    shape_.clear();

    topLeft_ = { constants::gridWidth / 2 - 2, 0 };
    tetrominoShape_ = newShape;
    col_ = determineColour(tetrominoShape_);
    currentRotation_ = 0;

    init();
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
    default:    // shouldn't get here - return an "error" colour
        return colours::instructionsText;
    }
}