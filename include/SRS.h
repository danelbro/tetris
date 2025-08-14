#pragma once

#include "TetrominoShape.h"
#include "Grid.h"
#include "GridPoint.h"

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

typedef std::tuple<int, char, size_t, size_t> srs_key;

bool operator==(srs_key& left, srs_key& right)
{
    return std::get<0>(left) == std::get<0>(right)
           && std::get<1>(left) == std::get<1>(right)
           && std::get<2>(left) == std::get<2>(right)
           && std::get<3>(left) == std::get<3>(right);
}

struct srs_hash {
    std::size_t operator()(const srs_key& k) const
    {
        return std::get<0>(k) ^ std::get<1>(k) ^ std::get<2>(k)
               ^ std::get<3>(k);
    }
};

typedef std::unordered_map<const srs_key, GridPoint, srs_hash> srs_map;

// clang-format off
inline srs_map
testDB
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
