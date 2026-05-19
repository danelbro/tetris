#include "TetrisApp.h"

#include <utl_SDLInterface.hpp>
#include <SDL3/SDL_main.h>

int main(int, char**)
{
    try {
        TetrisApp tetris{};
        tetris.run();
    }
    catch (utl::SdlException&) {
        return 1;
    }

    return 0;
}
