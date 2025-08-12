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

/// grey
inline const Colour gridBG{0xA3, 0xA3, 0xA3, 0xFF};

/// white
inline const Colour gridWalls{0xFF, 0xFF, 0xFF, 0xFF};

/// light blue
inline const Colour I_tetrominoCol{0x9E, 0xF2, 0xF4, 0xFF};

/// yellow
inline const Colour O_tetrominoCol{0xF8, 0xE0, 0x78, 0xFF};

/// purple
inline const Colour T_tetrominoCol{0xCF, 0x5C, 0xEE, 0xFF};

/// blue
inline const Colour J_tetrominoCol{0x1E, 0x66, 0xF6, 0xFF};

/// orange
inline const Colour L_tetrominoCol{0xE5, 0x5D, 0x11, 0xFF};

/// red
inline const Colour S_tetrominoCol{0xCB, 0x01, 0x01, 0xFF};

/// green
inline const Colour Z_tetrominoCol{0x03, 0x9B, 0x11, 0xFF};

}  // namespace colours
