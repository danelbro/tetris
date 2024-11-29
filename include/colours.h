// -*- C++ -*-
/// @file Contains constants represwnting useful colours (RGBA)

#pragma once

#include <utl_SDLInterface.hpp>

namespace colours {

using utl::Colour;

/// black
inline const Colour background{0x00, 0x00, 0x00, 0xFF};

/// white
inline const Colour titleText{0xFF, 0xFF, 0xFF, 0xFF};

/// white
inline const Colour instructionsText{0xFF, 0xFF, 0xFF, 0xFF};

/// white
inline const Colour gridWalls{0xFF, 0xFF, 0xFF, 0xFF};

}  // namespace colours
