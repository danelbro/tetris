#include <iostream>

#include "Tetromino.h"

#include "Cell.h"
#include "Grid.h"
#include "GridPoint.h"
#include "TetrisGame.h"
#include "TetrominoShape.h"
#include "colours.h"
#include "constants.h"
#include "flags.h"

#include <chrono>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utl_SDLInterface.hpp>
#include <utl_Vec2d.hpp>

// local constants
static const std::array<GridPoint, 4> T_Corners{
    {{0, 0}, {2, 0}, {2, 2}, {0, 2}}};

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

// structs and enums
enum class ORIENTATION
{
    VERTICAL,
    HORIZONTAL,
};

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

struct Orientation {
    ORIENTATION orientation{};
    int backRow{-1};
    int frontRow{-1};
    int backCol{-1};
    int frontCol{-1};
};

struct FilledCornerPositions {
    int backCorners{0};
    int frontCorners{0};
};

// declarations of static helper functions
static const utl::Colour& determineColour(const TetrominoShape&);
static bool isShapeInSpace(const TetrominoShape& shape, const size_t& rotation,
                           Grid& grid, const GridPoint& topLeft);
static bool test(TestPacket& testPacket, int testNo);
static std::vector<GridPoint> countFilledCorners(const GridPoint& origin,
                                                 Grid& grid);
static Orientation determineOrientation(size_t rotation);
static FilledCornerPositions
determinePositionOfFilledCorners(const Orientation& orientationPack,
                                 const std::vector<GridPoint>& filledCorners);

// member functions
Tetromino::Tetromino(TetrisGame* owner) : utl::Entity{}, owner_{owner}
{
    init();
}

void Tetromino::update(std::chrono::milliseconds, std::chrono::milliseconds)
{
    readShape();

    repositionInGridSpace(dropThisTick);
    dropThisTick = {0, 0};
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

const std::string& Tetromino::type() const
{
    return type_;
}

const utl::Vec2d& Tetromino::pos() const
{
    return pos_;
}

const utl::Size& Tetromino::size() const
{
    return size_;
}

utl::Stage& Tetromino::stage()
{
    if (!owner_)
        throw std::runtime_error("Tetromino has no owner!");
    return *owner_;
}

void Tetromino::set_pos(const utl::Vec2d& new_pos)
{
    pos_ = new_pos;
}

void Tetromino::reset(const TetrominoShape& newShape)
{
    shape_.clear();

    origin_ = {constants::gridWidth / 2 - 2, 0};
    tetrominoShape_ = newShape;
    col_ = determineColour(tetrominoShape_);
    currentRotation_ = 0;

    init();
    if (!isShapeInSpace(tetrominoShape_, currentRotation_, owner_->grid(),
                        origin_))
        owner_->grid().notifyLoss(*this);
}

void Tetromino::move(Translation move)
{
    repositionInGridSpace(move);
}

void Tetromino::rotate(flags::ROTATION rotation)
{
    int changeRotation{rotation == flags::ROTATION::CLOCKWISE ? 1 : -1};
    size_t new_rotation{
        static_cast<size_t>((static_cast<int>(currentRotation_) + changeRotation
                             + static_cast<int>(constants::rotations))
                            % static_cast<int>(constants::rotations))};

    TestPacket testPacket(tetrominoShape_, owner_->grid(), origin_,
                          currentRotation_, new_rotation);

    for (int testNo{1}; testNo <= constants::tests; ++testNo) {
        if (test(testPacket, testNo)) {
            currentRotation_ = new_rotation;
            origin_.x = testPacket.topLeft.x;
            origin_.y = testPacket.topLeft.y;
            break;
        }
    }
}

void Tetromino::tick_down()
{
    dropThisTick.y = 1;
}

void Tetromino::soft_drop()
{
    repositionInGridSpace({0, 1});
}

void Tetromino::setTopLeft(const GridPoint& point)
{
    origin_ = point;
}

const utl::Colour& Tetromino::colour() const
{
    return col_;
}

const TetrominoShape& Tetromino::shape() const
{
    return tetrominoShape_;
}

const size_t& Tetromino::currentRotation() const
{
    return currentRotation_;
}

const GridPoint& Tetromino::origin() const
{
    return origin_;
}

flags::TSpin Tetromino::checkTSpin() const
{
    if (tetrominoShape_ != T_tetromino)
        return flags::TSpin::NOSPIN;

    std::vector<GridPoint> filledCorners{
        countFilledCorners(origin_, owner_->grid())};

    if (filledCorners.size() < constants::filledCornersForTSpin)
        return flags::TSpin::NOSPIN;

    Orientation orientation{determineOrientation(currentRotation_)};

    FilledCornerPositions positions{
        determinePositionOfFilledCorners(orientation, filledCorners)};

    LOGF("TSpin! Front corners: %d\n", positions.frontCorners);

    if (positions.frontCorners == constants::frontCornersForTSpin)
        return flags::TSpin::TSPIN;
    else
        return flags::TSpin::MINI;
}

void Tetromino::init()
{
    shape_.reserve(constants::shapeWidth * constants::shapeHeight);
    for (size_t i{0}; i < constants::shapeWidth * constants::shapeHeight; ++i) {
        shape_.emplace_back(owner_);
    }

    readShape();
}

void Tetromino::readShape()
{
    for (auto& cell : shape_) cell.stopRendering();

    auto& current_shape{
        tetrominoShape_.at(static_cast<size_t>(currentRotation_))};

    for (const auto& cell : current_shape) {
        auto& activeCell = shape_[static_cast<size_t>(
            cell.x + cell.y * constants::shapeWidth)];
        activeCell.setColour(col_);
        activeCell.makeRender();
    }
}

void Tetromino::repositionInGridSpace(Translation translation)
{
    GridPoint checkPoint{origin_ + translation};
    if (isShapeInSpace(tetrominoShape_, currentRotation_, owner_->grid(),
                       checkPoint)) {
        origin_ += translation;
    } else if (translation.y > 0)
        owner_->grid().notifyBlockedTetro(*this);
}

void Tetromino::repositionInScreenSpace()
{
    set_pos({owner_->grid().innerTopLeftPt.x
                 + static_cast<double>(origin_.x * constants::cellWidth),
             owner_->grid().innerTopLeftPt.y
                 + static_cast<double>(origin_.y * constants::cellHeight)});

    for (size_t i{0}; i < shape_.size(); ++i) {
        size_t x{i % constants::shapeWidth};
        size_t y{i / constants::shapeHeight};
        float newX{static_cast<float>(pos_.x)
                   + constants::cellWidth * static_cast<float>(x)};
        float newY{static_cast<float>(pos_.y)
                   + constants::cellHeight * static_cast<float>(y)};
        shape_[i].update_rect(
            {newX, newY, constants::cellWidth, constants::cellHeight});
    }
}

// static helper functions
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
                           Grid& grid, const GridPoint& topLeft)
{
    for (const GridPoint& cell : shape.at(rotation)) {
        int x{topLeft.x + cell.x};
        int y{topLeft.y + cell.y};
        if (x >= constants::gridWidth || x < 0 || y >= constants::gridHeight
            || y < 0 || !grid.get({x, y}).isOpen())
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

static std::vector<GridPoint> countFilledCorners(const GridPoint& origin,
                                                 Grid& grid)
{
    std::vector<GridPoint> filledCorners{};
    filledCorners.reserve(constants::filledCornersForTSpin);

    for (const auto& corner : T_Corners) {
        int x{origin.x + corner.x};
        int y{origin.y + corner.y};
        if (x >= constants::gridWidth || x < 0 || y >= constants::gridHeight
            || y < 0 || !grid.get({x, y}).isOpen()) {
            filledCorners.emplace_back(corner);
        }
    }

    return filledCorners;
}

static Orientation determineOrientation(size_t rotation)
{
    Orientation op{};

    switch (rotation) {
    case 0:
        op.orientation = ORIENTATION::HORIZONTAL;
        op.backRow = 2;
        op.frontRow = 0;
        break;
    case 2:
        op.orientation = ORIENTATION::HORIZONTAL;
        op.backRow = 0;
        op.frontRow = 2;
        break;
    case 1:
        op.orientation = ORIENTATION::VERTICAL;
        op.backCol = 0;
        op.frontCol = 2;
        break;
    case 3:
        op.orientation = ORIENTATION::VERTICAL;
        op.backCol = 2;
        op.frontCol = 2;
        break;
    }

    return op;
}

static FilledCornerPositions
determinePositionOfFilledCorners(const Orientation& orientationPack,
                                 const std::vector<GridPoint>& filledCorners)
{
    FilledCornerPositions filledCornerPositions{};

    if (orientationPack.orientation == ORIENTATION::HORIZONTAL) {
        for (const auto& corner : filledCorners) {
            if (corner.y == orientationPack.backRow)
                filledCornerPositions.backCorners++;
            if (corner.y == orientationPack.frontRow)
                filledCornerPositions.frontCorners++;
        }
    } else if (orientationPack.orientation == ORIENTATION::VERTICAL) {
        for (const auto& corner : filledCorners) {
            if (corner.x == orientationPack.backCol)
                filledCornerPositions.backCorners++;
            if (corner.x == orientationPack.frontCol)
                filledCornerPositions.frontCorners++;
        }
    }

    return filledCornerPositions;
}
