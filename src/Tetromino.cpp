#include <iostream>

#include "Tetromino.h"

#include "Cell.h"
#include "Grid.h"
#include "GridPoint.h"
#include "SRS.h"
#include "TetrominoShape.h"
#include "colours.h"
#include "constants.h"
#include "flags.h"

#include <cstddef>
#include <unordered_map>
#include <utl_SDLInterface.hpp>
#include <utl_Vec2d.hpp>
#include <vector>

// clang-format off
static const std::unordered_map<SRSKey, GridPoint> testDB
{
    // J, L, S, T Z - test 1
    {{1,'O',0,1},{ 0, 0}},
    {{1,'O',1,0},{ 0, 0}},
    {{1,'O',1,2},{ 0, 0}},
    {{1,'O',2,1},{ 0, 0}},
    {{1,'O',2,3},{ 0, 0}},
    {{1,'O',3,2},{ 0, 0}},
    {{1,'O',3,0},{ 0, 0}},
    {{1,'O',0,3},{ 0, 0}},

    // I - test 1
    {{1,'I',0,1},{ 0, 0}},
    {{1,'I',1,0},{ 0, 0}},
    {{1,'I',1,2},{ 0, 0}},
    {{1,'I',2,1},{ 0, 0}},
    {{1,'I',2,3},{ 0, 0}},
    {{1,'I',3,2},{ 0, 0}},
    {{1,'I',3,0},{ 0, 0}},
    {{1,'I',0,3},{ 0, 0}},

    // J, L, S, T Z - test 2
    {{2,'O',0,1},{-1, 0}},
    {{2,'O',1,0},{ 1, 0}},
    {{2,'O',1,2},{ 1, 0}},
    {{2,'O',2,1},{-1, 0}},
    {{2,'O',2,3},{ 1, 0}},
    {{2,'O',3,2},{-1, 0}},
    {{2,'O',3,0},{-1, 0}},
    {{2,'O',0,3},{ 1, 0}},

    // I - test 2
    {{2,'I',0,1},{-2, 0}},
    {{2,'I',1,0},{ 2, 0}},
    {{2,'I',1,2},{-1, 0}},
    {{2,'I',2,1},{ 1, 0}},
    {{2,'I',2,3},{ 2, 0}},
    {{2,'I',3,2},{-2, 0}},
    {{2,'I',3,0},{ 1, 0}},
    {{2,'I',0,3},{-1, 0}},

    // J, L, S, T Z - test 3
    {{3,'O',0,1},{-1,-1}},
    {{3,'O',1,0},{ 1, 1}},
    {{3,'O',1,2},{ 1, 1}},
    {{3,'O',2,1},{-1,-1}},
    {{3,'O',2,3},{ 1,-1}},
    {{3,'O',3,2},{-1, 1}},
    {{3,'O',3,0},{-1, 1}},
    {{3,'O',0,3},{ 1,-1}},

    // I - test 3
    {{3,'I',0,1},{ 1, 0}},
    {{3,'I',1,0},{-1, 0}},
    {{3,'I',1,2},{ 2, 0}},
    {{3,'I',2,1},{-2, 0}},
    {{3,'I',2,3},{-1, 0}},
    {{3,'I',3,2},{ 1, 0}},
    {{3,'I',3,0},{-2, 0}},
    {{3,'I',0,3},{ 2, 0}},

    // J, L, S, T Z - test 4
    {{4,'O',0,1},{ 0, 2}},
    {{4,'O',1,0},{ 0,-2}},
    {{4,'O',1,2},{ 0,-2}},
    {{4,'O',2,1},{ 0, 2}},
    {{4,'O',2,3},{ 0, 2}},
    {{4,'O',3,2},{ 0,-2}},
    {{4,'O',3,0},{ 0,-2}},
    {{4,'O',0,3},{ 0, 2}},

    // I - test 4
    {{4,'I',0,1},{-2, 1}},
    {{4,'I',1,0},{ 2,-1}},
    {{4,'I',1,2},{-1,-2}},
    {{4,'I',2,1},{ 1, 2}},
    {{4,'I',2,3},{ 2,-1}},
    {{4,'I',3,2},{-2, 1}},
    {{4,'I',3,0},{ 1, 2}},
    {{4,'I',0,3},{-1,-2}},

    // J, L, S, T Z - test 5
    {{5,'O',0,1},{-1, 2}},
    {{5,'O',1,0},{ 1,-2}},
    {{5,'O',1,2},{ 1,-2}},
    {{5,'O',2,1},{-1, 2}},
    {{5,'O',2,3},{ 1, 2}},
    {{5,'O',3,2},{-1,-2}},
    {{5,'O',3,0},{-1,-2}},
    {{5,'O',0,3},{ 1, 2}},

    // I - test 5
    {{5,'I',0,1},{ 1,-2}},
    {{5,'I',1,0},{-1, 2}},
    {{5,'I',1,2},{ 2, 1}},
    {{5,'I',2,1},{-2,-1}},
    {{5,'I',2,3},{-1, 2}},
    {{5,'I',3,2},{ 1,-2}},
    {{5,'I',3,0},{-2,-1}},
    {{5,'I',0,3},{ 2, 1}},

};
// clang-format on

static const utl::Colour& determineColour(const TetrominoShape&);
static bool isShapeInSpace(const TetrominoShape& shape, const size_t& rotation,
                           const Grid& grid, const GridPoint& topLeft);
static bool test(TestPacket& testPacket, int testNo);

Tetromino::Tetromino(utl::Box& screen, Grid& grid, const GridPoint& grid_point,
                     const utl::Colour& colour,
                     const TetrominoShape& tetrominoShape)
    : utl::Entity{flags::ENTITIES_MAP.at(flags::ENTITIES::TETROMINO),
                  screen,
                  {}},
      tetrominoShape_{tetrominoShape}, grid_{grid}, topLeft_{grid_point},
      shape_{}, col_{colour}, tickTime{constants::initialTickTime},
      timeSinceTick{0.0}, rotationTimer{0.0}, rotationLengthInSecs{0.09},
      moveTimer{0.0}, moveLengthInSecs{0.08}, dropTimer{0.0},
      dropLengthInSecs{0.05}, isRotating{false}, isMoving{false},
      isDropping{false}, currentRotation_{0}
{
    init();
}

void Tetromino::init()
{
    shape_.reserve(constants::shapeWidth * constants::shapeHeight);
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
    for (auto& cell : shape_) cell.stopRendering();

    auto& current_shape{
        tetrominoShape_.at(static_cast<size_t>(currentRotation_))};

    for (const auto& cell : current_shape) {
        shape_[static_cast<size_t>(cell.x + cell.y * constants::shapeWidth)]
            .makeRender();
    }
}

void Tetromino::repositionInGridSpace(int x, int y)
{
    GridPoint checkPoint{topLeft_.x + x, topLeft_.y + y};
    if (isShapeInSpace(tetrominoShape_, currentRotation_, grid_, checkPoint)) {
        topLeft_.x += x;
        topLeft_.y += y;
    } else if (y > 0)
        grid_.bakeActiveTetromino(*this);
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
    if (isMoving)
        return;

    repositionInGridSpace(dir, 0);

    isMoving = true;
}

void Tetromino::rotate(int dir)
{
    if (isRotating || tetrominoShape_.id == 'O')
        return;

    size_t new_rotation{
        static_cast<size_t>((static_cast<int>(currentRotation_) + dir
                             + static_cast<int>(constants::rotations))
                            % static_cast<int>(constants::rotations))};

    TestPacket testPacket(tetrominoShape_, grid_, topLeft_, currentRotation_,
                          new_rotation);

    for (int testNo{1}; testNo <= constants::tests; ++testNo) {
        if (test(testPacket, testNo)) {
            currentRotation_ = new_rotation;
            topLeft_.x = testPacket.topLeft.x;
            topLeft_.y = testPacket.topLeft.y;
            isRotating = true;
            break;
        }
    }
}

void Tetromino::soft_drop()
{
    if (isDropping)
        return;

    repositionInGridSpace(0, 1);

    isDropping = true;
}

void Tetromino::reset(const TetrominoShape& newShape)
{
    shape_.clear();

    topLeft_ = {constants::gridWidth / 2 - 2, 0};
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
    default:  // shouldn't get here - return an "error" colour
        return colours::instructionsText;
    }
}

static bool isShapeInSpace(const TetrominoShape& shape, const size_t& rotation,
                           const Grid& grid, const GridPoint& topLeft)
{
    for (const GridPoint& cell : shape.at(rotation)) {
        int x{topLeft.x + cell.x};
        int y{topLeft.y + cell.y};
        if (x >= constants::gridWidth || x < 0 || y >= constants::gridHeight
            || y < 0 || !grid.get(x, y).isOpen())
            return false;
    }
    return true;
}

// Super Rotation System wall kick stuff vbelow
static bool test(TestPacket& tp, int testNo)
{
    char shapeType{};
    switch (tp.shape.id) {
    case 'J':
    case 'L':
    case 'S':
    case 'Z':
    case 'T':
        shapeType = 'O';
        break;
    case 'I':
        shapeType = 'I';
        break;
    default:
        shapeType = 'E';
        break;
    }

    SRSKey key(testNo, shapeType, tp.currentRotation, tp.newRotation);
    GridPoint shift{testDB.at(key)};
    GridPoint testPoint{tp.topLeft.x + shift.x, tp.topLeft.y + shift.y};

    if (isShapeInSpace(tp.shape, tp.newRotation, tp.grid, testPoint)) {
        tp.topLeft.x += shift.x;
        tp.topLeft.y += shift.y;
        return true;
    }

    return false;
}
