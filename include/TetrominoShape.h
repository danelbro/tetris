#pragma once

#include "GridPoint.h"
#include "constants.h"

#include <array>

struct TetrominoShape {
    TetrominoShape(
        std::array<
        std::array<GridPoint, constants::minos>,
        constants::rotations> newShape,
        char newID) : shape{newShape}, id{ newID } {
    }

    bool operator==(const TetrominoShape& other) const {
        return other.id == id;
    }

    const std::array<GridPoint, constants::minos>& at(const size_t i) const
    {
        return shape.at(i);
    }


    std::array<std::array<GridPoint, constants::minos>, constants::rotations>
        shape;
    char id;
};

inline const TetrominoShape I_tetromino{
    {{
      {{{0, 1}, {1, 1}, {2, 1}, {3, 1}}},
      {{{2, 0}, {2, 1}, {2, 2}, {2, 3}}},
      {{{0, 2}, {1, 2}, {2, 2}, {3, 2}}},
      {{{1, 0}, {1, 1}, {1, 2}, {1, 3}}}
        }},'I'
};

inline const TetrominoShape O_tetromino{
    {{
      {{{1, 0}, {2, 0}, {2, 1}, {1, 1}}},
      {{{1, 0}, {2, 0}, {2, 1}, {1, 1}}},
      {{{1, 0}, {2, 0}, {2, 1}, {1, 1}}},
      {{{1, 0}, {2, 0}, {2, 1}, {1, 1}}}
        }}, 'O'
};

inline const TetrominoShape T_tetromino{
    {{
      {{{1, 0}, {0, 1}, {1, 1}, {2, 1}}},
      {{{1, 0}, {1, 1}, {2, 1}, {1, 2}}},
      {{{0, 1}, {1, 1}, {2, 1}, {1, 2}}},
      {{{1, 0}, {0, 1}, {1, 1}, {1, 2}}}
        }}, 'T'
};

inline const TetrominoShape J_tetromino{
    {{{{{0, 0}, {0, 1}, {1, 1}, {2, 1}}},
      {{{1, 0}, {2, 0}, {1, 1}, {1, 2}}},
      {{{0, 1}, {1, 1}, {2, 1}, {2, 2}}},
      {{{1, 0}, {1, 1}, {1, 2}, {0, 2}}}
        }}, 'J'
};

inline const TetrominoShape L_tetromino{
    {{{{{2, 0}, {0, 1}, {1, 1}, {2, 1}}},
      {{{1, 0}, {1, 1}, {1, 2}, {2, 2}}},
      {{{0, 1}, {1, 1}, {2, 1}, {0, 2}}},
      {{{0, 0}, {1, 0}, {1, 1}, {1, 2}}}
        }}, 'L'
};

inline const TetrominoShape S_tetromino{
    {{{{{1, 0}, {2, 0}, {0, 1}, {1, 1}}},
      {{{1, 0}, {1, 1}, {2, 1}, {2, 2}}},
      {{{1, 1}, {2, 1}, {0, 2}, {1, 2}}},
      {{{0, 0}, {0, 1}, {1, 1}, {1, 2}}}
        }}, 'S'
};

inline const TetrominoShape Z_tetromino{
    {{{{{0, 0}, {1, 0}, {1, 1}, {2, 1}}},
      {{{2, 0}, {1, 1}, {2, 1}, {1, 2}}},
      {{{0, 1}, {1, 1}, {1, 2}, {2, 2}}},
      {{{1, 0}, {0, 1}, {1, 1}, {0, 2}}}
        }}, 'Z'
};
