#include "TetrisApp.h"

#include "TitleScreen.h"
#include "EndScreen.h"
#include "TetrisGame.h"
#include "flags.h"

#include <stdexcept>
#include <string>
#include <utl_Application.hpp>
#include <utl_SDLInterface.hpp>

// Ensures:
// 1. SDL is initialised
// 2. A rendering context exists
// 3. A stage manager exists with at least one stage
TetrisApp::TetrisApp()
    : utl::Application{},
      context_{utl::create_window_with_renderer(title_, screen_space_.w,
                                                screen_space_.h, windowFlags_)},
      stage_manager_{this}
{
    auto isSDLInitialised{
        utl::init_SDL(title_, version_, identifier_, sdlFlags_)};
    if (!isSDLInitialised)
        throw utl::SdlException{std::string{
            "Couldn't initialise SDL! SDL_Error: ", SDL_GetError()}};

    context_.renderer.setVSync(1);
    LOG("Set VSync\n");
    context_.renderer.setDrawingBlendMode(SDL_BLENDMODE_BLEND);

    stage_manager_.add_stage<TitleScreen>(*this,
        flags::STAGES_MAP.at(flags::STAGES::TITLE_SCREEN));
    stage_manager_.set_current_stage(
        flags::STAGES_MAP.at(flags::STAGES::TITLE_SCREEN));
    stage_manager_.set_next_stage(
        flags::STAGES_MAP.at(flags::STAGES::TITLE_SCREEN));
    LOG("Created TetrisApp\n");
}

TetrisApp::~TetrisApp()
{
    utl::quit_sdl();
    LOG("Quit SDL\n");
    LOG("Destroying TetrisApp\n");
}

const std::string& TetrisApp::title() const
{
    return title_;
}

const std::string& TetrisApp::version() const
{
    return version_;
}

const std::string& TetrisApp::identifier() const
{
    return identifier_;
}

utl::Box& TetrisApp::screen()
{
    return screen_space_;
}

uint32_t TetrisApp::flags()
{
    return sdlFlags_;
}

uint32_t TetrisApp::windowFlags()
{
    return windowFlags_;
}

utl::Window& TetrisApp::window()
{
    return context_.window;
}

utl::Renderer& TetrisApp::renderer()
{
    return context_.renderer;
}

utl::StageManager& TetrisApp::stageManager()
{
    return stage_manager_;
}

void TetrisApp::trigger_stage_change(const std::string& next)
{
    switch (flags::STAGES_STRING_MAP.at(next)) {
    case flags::STAGES::TITLE_SCREEN:
        stage_manager_.add_stage<TitleScreen>(*this, next);
        break;
    case flags::STAGES::TETRIS:
        stage_manager_.add_stage<TetrisGame>(*this, next);
        break;
    case flags::STAGES::END_SCREEN: {
        TetrisGame* currentTetris{
            dynamic_cast<TetrisGame*>(stage_manager_.get_current_stage())};
        if (!currentTetris)
            throw std::runtime_error(
                "attempted to create END_SCREEN from inappropriate context");

        ScoresPacket scores{currentTetris->getScore(),
                            currentTetris->getLevel(),
                            currentTetris->getLines()};

        stage_manager_.add_stage<EndScreen>(*this, next, currentTetris->grid(),
                                            scores);
        break;
    }
    case flags::STAGES::QUIT:
        break;
    default:
        break;
    }

    LOGF("Going to %s stage\n", next.c_str());
}
