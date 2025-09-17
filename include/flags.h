// -*- C++ -*-
/// @file Enums to identify Entities and Stages, along with global constant maps
/// to go to and from string representations

#pragma once

#include <string>
#include <unordered_map>

namespace flags {

enum class ENTITIES
{
    GRID,
    CELL,
    DISPLAYBOX,
    TETROMINO,
    GHOST_PIECE,
    ENTITIES_TOTAL
};

inline const std::unordered_map<ENTITIES, std::string> ENTITIES_MAP{
    {ENTITIES::GRID, "GRID"},
    {ENTITIES::CELL, "CELL"},
    {ENTITIES::DISPLAYBOX, "DISPLAYBOX"},
    {ENTITIES::TETROMINO, "TETROMINO"},
    {ENTITIES::GHOST_PIECE, "GHOST_PIECE"},
};
inline const std::unordered_map<std::string, ENTITIES> ENTITIES_STRING_MAP{
    {"GRID", ENTITIES::GRID},
    {"CELL", ENTITIES::CELL},
    {"DISPLAYBOX", ENTITIES::DISPLAYBOX},
    {"TETROMINO", ENTITIES::TETROMINO},
    {"GHOST_PIECE", ENTITIES::GHOST_PIECE},
};

enum class STAGES
{
    TITLE_SCREEN,
    TETRIS,
    END_SCREEN,
    STAGES_TOTAL,
    QUIT
};

inline const std::unordered_map<STAGES, std::string> STAGES_MAP{
    {STAGES::TITLE_SCREEN, "TITLE_SCREEN"},
    {STAGES::TETRIS, "TETRIS"},
    {STAGES::END_SCREEN, "END_SCREEN"},
    {STAGES::QUIT, "QUIT"}};

inline const std::unordered_map<std::string, STAGES> STAGES_STRING_MAP{
    {"TITLE_SCREEN", STAGES::TITLE_SCREEN},
    {"TETRIS", STAGES::TETRIS},
    {"END_SCREEN", STAGES::END_SCREEN},
    {"QUIT", STAGES::QUIT}};

enum class MOVE
{
    PAUSE,
    QUIT,
    HOLD,
    ROTATE,
    MOVE,
    SOFTDROP,
    HARDDROP,
};

}  // namespace flags
