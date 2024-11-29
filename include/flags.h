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
    TETROMINO,
    ENTITIES_TOTAL
};

inline const std::unordered_map<ENTITIES, std::string> ENTITIES_MAP{
    {ENTITIES::GRID, "GRID"},
    {ENTITIES::CELL, "CELL"},
    {ENTITIES::TETROMINO, "TETROMINO"}};

inline const std::unordered_map<std::string, ENTITIES> ENTITIES_STRING_MAP{
    {"GRID", ENTITIES::GRID},
    {"CELL", ENTITIES::CELL},
    {"TETROMINO", ENTITIES::TETROMINO}};

enum class STAGES
{
    TITLE_SCREEN,
    TETRIS,
    STAGES_TOTAL,
    QUIT
};

inline const std::unordered_map<STAGES, std::string> STAGES_MAP{
    {STAGES::TITLE_SCREEN, "TITLE_SCREEN"},
    {STAGES::TETRIS, "TETRIS"},
    {STAGES::QUIT, "QUIT"}};

inline const std::unordered_map<std::string, STAGES> STAGES_STRING_MAP{
    {"TITLE_SCREEN", STAGES::TITLE_SCREEN},
    {"TETRIS", STAGES::TETRIS},
    {"QUIT", STAGES::QUIT}};

}  // namespace flags
