#include "GridPoint.h"
#include "constants.h"

#include <array>

struct TetrominoShape {
    std::array<std::array<GridPoint, constants::minos>, constants::rotations>
        shape;

    std::array<GridPoint, constants::minos>& operator[](const size_t i) { return shape[i]; }
};

inline const TetrominoShape I_tetromino = {
    {{{{{0, 1}, {1, 1}, {2, 1}, {3, 1}}},
      {{{2, 0}, {2, 1}, {2, 2}, {2, 3}}},
      {{{0, 2}, {1, 2}, {2, 2}, {3, 2}}},
      {{{1, 0}, {1, 1}, {1, 2}, {1, 3}}}}}};

/*                                      {{{{0, 1}, {1, 1}, {2, 1}, {3, 1}}, */
/*                                        {{2, 0}, {2, 1}, {2, 2}, {2, 3}}, */
/*                                        {{0, 2}, {1, 2}, {2, 2}, {3, 2}}, */
/*                                        {{1, 0}, {1, 1}, {1, 2}, {1, 3}}}; */

inline const TetrominoShape O_tetromino{
    false, false, false, false, false, true,  true,  false,
    false, true,  true,  false, false, false, false, false};

inline const TetrominoShape T_tetromino{
    false, true,  false, false, true,  true,  true,  false,
    false, false, false, false, false, false, false, false};

inline const TetrominoShape J_tetromino{
    true,  false, false, false, true,  true,  true,  false,
    false, false, false, false, false, false, false, false};

inline const TetrominoShape L_tetromino{
    false, false, true,  false, true,  true,  true,  false,
    false, false, false, false, false, false, false, false};

inline const TetrominoShape S_tetromino{
    false, true,  true,  false, true,  true,  false, false,
    false, false, false, false, false, false, false, false};

inline const TetrominoShape Z_tetromino{
    true,  true,  false, false, false, true,  true,  false,
    false, false, false, false, false, false, false, false};
