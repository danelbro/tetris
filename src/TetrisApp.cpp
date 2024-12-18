#include "TetrisApp.h"

#include "TetrisGame.h"
#include "TitleScreen.h"
#include "constants.h"
#include "flags.h"

#include <string>
#include <utl_Application.hpp>
#include <utl_SDLInterface.hpp>

TetrisApp::TetrisApp()
    : utl::Application{constants::tetrisTitle,      constants::tetrisVersion,
                       constants::tetrisIdentifier, constants::screenWidth,
                       constants::screenHeight,     constants::sdlFlags,
                       constants::windowFlags}
{
    m_renderer.setVSync(1);
    LOG("Set VSync\n");

    m_stageManager.add_stage<TitleScreen>(
        flags::STAGES_MAP.at(flags::STAGES::TITLE_SCREEN), m_screenSpace,
        m_windowID, m_renderer);
    m_stageManager.set_next_stage(
        flags::STAGES_MAP.at(flags::STAGES::TITLE_SCREEN));
    m_stageManager.set_current_stage(
        flags::STAGES_MAP.at(flags::STAGES::TITLE_SCREEN));
    LOG("Created TetrisApp\n");
}

TetrisApp::~TetrisApp()
{
    LOG("Destroying TetrisApp\n");
}

void TetrisApp::trigger_stage_change(const std::string& next)
{
    utl::Box& screen{m_stageManager.get_current_stage()->screen()};
    uint32_t windowID{m_stageManager.get_current_stage()->windowID()};
    utl::Renderer& renderer{m_stageManager.get_current_stage()->renderer()};

    switch (flags::STAGES_STRING_MAP.at(next)) {
    case flags::STAGES::TITLE_SCREEN:
        m_stageManager.add_stage<TitleScreen>(next, screen, windowID, renderer);
        break;
    case flags::STAGES::TETRIS:
        m_stageManager.add_stage<TetrisGame>(next, screen, windowID, renderer);
        break;
    case flags::STAGES::QUIT:
        break;
    default:
        break;
    }

    LOGF("Going to %s stage\n", next.c_str());
}
