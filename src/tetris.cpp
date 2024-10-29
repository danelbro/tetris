#include "TetrisApp.h"

#include <SDL3/SDL_main.h>
#include <utl_SDLInterface.hpp>
#include <utl_utility.hpp>

int main(int, char**)
{
    utl::init(SDL_INIT_VIDEO);

    {
        const std::string title{"Tetris"};
        constexpr int screenWidth{640};
        constexpr int screenHeight{480};
        constexpr unsigned windowFlags{0};

        TetrisApp tetris{title, screenWidth, screenHeight, windowFlags};
        tetris.run();
    }

    utl::quit_sdl();
}
