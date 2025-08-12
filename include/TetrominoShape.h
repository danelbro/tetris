#pragma once

#include "GridPoint.h"
#include "constants.h"

#include <array>

struct TetrominoShape {
    std::array<std::array<GridPoint, constants::minos>, constants::rotations>
        shape;

    const std::array<GridPoint, constants::minos>& at(const size_t i) const
    {
        return shape.at(i);
    }
};

inline const TetrominoShape I_tetromino{
    {{{{{0, 1}, {1, 1}, {2, 1}, {3, 1}}},
      {{{2, 0}, {2, 1}, {2, 2}, {2, 3}}},
      {{{0, 2}, {1, 2}, {2, 2}, {3, 2}}},
      {{{1, 0}, {1, 1}, {1, 2}, {1, 3}}}}}
};

inline const TetrominoShape O_tetromino{
    {{{{{1, 0}, {2, 0}, {2, 1}, {1, 1}}},
      {{{1, 0}, {2, 0}, {2, 1}, {1, 1}}},
      {{{1, 0}, {2, 0}, {2, 1}, {1, 1}}},
      {{{1, 0}, {2, 0}, {2, 1}, {1, 1}}}}}
};

inline const TetrominoShape T_tetromino{
    {{{{{1, 0}, {0, 1}, {1, 1}, {2, 1}}},
      {{{1, 0}, {1, 1}, {2, 1}, {1, 2}}},
      {{{0, 1}, {1, 1}, {2, 1}, {1, 2}}},
      {{{1, 0}, {0, 1}, {1, 1}, {1, 2}}}}}
};

inline const TetrominoShape J_tetromino{
    {{{{{0, 0}, {0, 1}, {1, 1}, {2, 1}}},
      {{{1, 0}, {2, 0}, {1, 1}, {1, 2}}},
      {{{0, 1}, {1, 1}, {2, 1}, {2, 2}}},
      {{{1, 0}, {1, 1}, {1, 2}, {0, 2}}}}}
};

inline const TetrominoShape L_tetromino{
    {{{{{2, 0}, {0, 1}, {1, 1}, {2, 1}}},
      {{{1, 0}, {1, 1}, {1, 2}, {2, 2}}},
      {{{0, 1}, {1, 1}, {2, 1}, {0, 2}}},
      {{{0, 0}, {1, 0}, {1, 1}, {1, 2}}}}}
};

inline const TetrominoShape S_tetromino{
    {{{{{1, 0}, {2, 0}, {0, 1}, {1, 1}}},
      {{{1, 0}, {1, 1}, {2, 1}, {2, 2}}},
      {{{1, 1}, {2, 1}, {0, 2}, {1, 2}}},
      {{{0, 0}, {0, 1}, {1, 1}, {1, 2}}}}}
};

inline const TetrominoShape Z_tetromino{
    {{{{{0, 0}, {1, 0}, {1, 1}, {2, 1}}},
      {{{2, 0}, {1, 1}, {2, 1}, {1, 2}}},
      {{{0, 1}, {1, 1}, {1, 2}, {2, 2}}},
      {{{1, 0}, {0, 1}, {1, 1}, {0, 2}}}}}
};
