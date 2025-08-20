#include <iostream>

#include "Tetromino.h"

#include "Cell.h"
#include "Grid.h"
#include "GridPoint.h"
#include "TetrominoShape.h"
#include "colours.h"
#include "constants.h"
#include "flags.h"

#include <cstddef>
#include <string>
#include <unordered_map>
#include <utl_SDLInterface.hpp>
#include <utl_Vec2d.hpp>
#include <vector>

struct TestPacket {
    TestPacket(TetrominoShape& newShape, Grid& newGrid, GridPoint& point,
               size_t currentRot, size_t newRot)
        : shape{newShape}, grid{newGrid}, topLeft{point},
          currentRotation{currentRot}, newRotation{newRot}
    {}

    TetrominoShape shape;
    Grid& grid;
    GridPoint topLeft;
    size_t currentRotation;
    size_t newRotation;
};

// clang-format off
const std::unordered_map<std::string, GridPoint> testDB
{
    // J, L, S, T Z - test 1
    {"1O01",{ 0, 0}},
    {"1O10",{ 0, 0}},
    {"1O12",{ 0, 0}},
    {"1O21",{ 0, 0}},
    {"1O23",{ 0, 0}},
    {"1O32",{ 0, 0}},
    {"1O30",{ 0, 0}},
    {"1O03",{ 0, 0}},

    // I - test 1
    {"1I01",{ 0, 0}},
    {"1I10",{ 0, 0}},
    {"1I12",{ 0, 0}},
    {"1I21",{ 0, 0}},
    {"1I23",{ 0, 0}},
    {"1I32",{ 0, 0}},
    {"1I30",{ 0, 0}},
    {"1I03",{ 0, 0}},

    // J, L, S, T Z - test 2
    {"2O01",{-1, 0}},
    {"2O10",{ 1, 0}},
    {"2O12",{ 1, 0}},
    {"2O21",{-1, 0}},
    {"2O23",{ 1, 0}},
    {"2O32",{-1, 0}},
    {"2O30",{-1, 0}},
    {"2O03",{ 1, 0}},

    // I - test 2
    {"2I01",{-2, 0}},
    {"2I10",{ 2, 0}},
    {"2I12",{-1, 0}},
    {"2I21",{ 1, 0}},
    {"2I23",{ 2, 0}},
    {"2I32",{-2, 0}},
    {"2I30",{ 1, 0}},
    {"2I03",{-1, 0}},

    // J, L, S, T Z - test 3
    {"3O01",{-1,-1}},
    {"3O10",{ 1, 1}},
    {"3O12",{ 1, 1}},
    {"3O21",{-1,-1}},
    {"3O23",{ 1,-1}},
    {"3O32",{-1, 1}},
    {"3O30",{-1, 1}},
    {"3O03",{ 1,-1}},

    // I - test 3
    {"3I01",{ 1, 0}},
    {"3I10",{-1, 0}},
    {"3I12",{ 2, 0}},
    {"3I21",{-2, 0}},
    {"3I23",{-1, 0}},
    {"3I32",{ 1, 0}},
    {"3I30",{-2, 0}},
    {"3I03",{ 2, 0}},

    // J, L, S, T Z - test 4
    {"4O01",{ 0, 2}},
    {"4O10",{ 0,-2}},
    {"4O12",{ 0,-2}},
    {"4O21",{ 0, 2}},
    {"4O23",{ 0, 2}},
    {"4O32",{ 0,-2}},
    {"4O30",{ 0,-2}},
    {"4O03",{ 0, 2}},

    // I - test 4
    {"4I01",{-2, 1}},
    {"4I10",{ 2,-1}},
    {"4I12",{-1,-2}},
    {"4I21",{ 1, 2}},
    {"4I23",{ 2,-1}},
    {"4I32",{-2, 1}},
    {"4I30",{ 1, 2}},
    {"4I03",{-1,-2}},

    // J, L, S, T Z - test 5
    {"5O01",{-1, 2}},
    {"5O10",{ 1,-2}},
    {"5O12",{ 1,-2}},
    {"5O21",{-1, 2}},
    {"5O23",{ 1, 2}},
    {"5O32",{-1,-2}},
    {"5O30",{-1,-2}},
    {"5O03",{ 1, 2}},

    // I - test 5
    {"5I01",{ 1,-2}},
    {"5I10",{-1, 2}},
    {"5I12",{ 2, 1}},
    {"5I21",{-2,-1}},
    {"5I23",{-1, 2}},
    {"5I32",{ 1,-2}},
    {"5I30",{-2,-1}},
    {"5I03",{ 2, 1}}
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
      shape_{}, col_{colour}, tickTime_{constants::initialTickTime},
      timeSinceTick{0.0}, currentRotation_{0}, size_{}  // todo
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

    timeSinceTick += dt;
    if (timeSinceTick >= tickTime_) {
        timeSinceTick = 0.0;
        repositionInGridSpace(0, 1);
    }
}

void Tetromino::changeTickTime(double newTickTime)
{
    tickTime_ = newTickTime;
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
        grid_.notifyBlockedTetro(*this);
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
    repositionInGridSpace(dir, 0);
}

void Tetromino::rotate(int dir)
{
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
            break;
        }
    }
}

void Tetromino::soft_drop()
{
    repositionInGridSpace(0, 1);
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

static bool test(TestPacket& tp, int testNo)
{
    char shapeType{};
    switch (tp.shape.id) {
    case 'I':
        shapeType = 'I';
        break;
    case 'J':
    case 'L':
    case 'S':
    case 'Z':
    case 'T':
        shapeType = 'O';  // in this context 'O' stands for 'other', because...
        break;
    default:  // the O tetromino doesn't rotate
        return false;
    }

    std::stringstream keyStream{};

    keyStream << testNo << shapeType << tp.currentRotation << tp.newRotation;

    std::string key{keyStream.str()};
    GridPoint shift{testDB.at(key)};
    GridPoint testPoint{tp.topLeft.x + shift.x, tp.topLeft.y + shift.y};

    if (isShapeInSpace(tp.shape, tp.newRotation, tp.grid, testPoint)) {
        tp.topLeft.x += shift.x;
        tp.topLeft.y += shift.y;
        return true;
    }

    return false;
}
