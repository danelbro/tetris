#pragma once

#include "Grid.h"
#include "GridPoint.h"
#include "TetrominoShape.h"

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

struct SRSKey {
    int test_;
    char type_;
    size_t currentRotation_;
    size_t newRotation_;

    SRSKey(int test, char type, size_t currentRotation, size_t newRotation)
        : test_{test}, type_{type}, currentRotation_{currentRotation},
          newRotation_{newRotation}
    {}
};

bool operator==(const SRSKey& left, const SRSKey& right);

template<>
struct std::hash<SRSKey> {
    size_t operator()(const SRSKey& k) const
    {
        auto h1{std::hash<int>()(k.test_)};
        auto h2{std::hash<char>()(k.type_)};
        auto h3{std::hash<size_t>()(k.currentRotation_)};
        auto h4{std::hash<size_t>()(k.newRotation_)};

        return (((h1 >> 1) ^ (h2 << 1)) << 1) ^ (((h3 >> 1) ^ (h4 << 1)) << 1);
    }
};
