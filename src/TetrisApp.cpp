#include "TetrisApp.h"

#include <string>
#include <utl_Application.hpp>
#include <utl_SDLInterface.hpp>

const std::string tetrisTitle{"Tetris"};
const std::string tetrisVersion{"0.0.1"};
const std::string tetrisIdentifier{"com.tetris.tetris"};
constexpr int screenWidth{1280};
constexpr int screenHeight{720};
constexpr unsigned sdlFlags{SDL_INIT_VIDEO};
constexpr unsigned windowFlags{0};

TetrisApp::TetrisApp()
    : utl::Application{tetrisTitle, tetrisVersion, tetrisIdentifier,
                       screenWidth, screenHeight,  sdlFlags,
                       windowFlags}
{
    LOG("Created TetrisApp\n");
}

TetrisApp::~TetrisApp()
{
    LOG("Destroying TetrisApp\n");
}

void TetrisApp::run()
{
    // m_stageManager.run(); }
}

void TetrisApp::trigger_stage_change(const std::string&) {}
