// -*- C++ -*-
/// @file Enums to identify Entities and Stages, along with global constant maps
/// to go to and from string representations

#pragma once

#include <string>
#include <unordered_map>

namespace flags {

enum class STAGES
{
    TITLE_SCREEN,
    STAGES_TOTAL,
    QUIT
};

inline const std::unordered_map<STAGES, std::string> STAGES_MAP{
    {STAGES::TITLE_SCREEN, "TITLE_SCREEN"}, {STAGES::QUIT, "QUIT"}};

inline const std::unordered_map<std::string, STAGES> STAGES_STRING_MAP{
    {"TITLE_SCREEN", STAGES::TITLE_SCREEN}, {"QUIT", STAGES::QUIT}};

}  // namespace flags
