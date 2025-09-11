// -*- C++ -*- //
/// @file An application that plays a tetris clone

#pragma once

#include "constants.h"

#include <cstdint>
#include <string>
#include <utl_Application.hpp>
#include <utl_Box.hpp>
#include <utl_SDLInterface.hpp>
#include <utl_StageManager.hpp>

class TetrisApp : public utl::Application {
public:
    /// Initialises the Application and adds a TitleScreen Stage to the
    /// StageManager.
    TetrisApp();
    ~TetrisApp();
    TetrisApp(const TetrisApp&) = delete;
    TetrisApp& operator=(const TetrisApp&) = delete;
    TetrisApp(TetrisApp&&) = delete;
    TetrisApp& operator=(TetrisApp&&) = delete;

    const std::string& title() const override final;
    const std::string& version() const override final;
    const std::string& identifier() const override final;
    utl::Box& screen() override final;
    uint32_t flags() override final;
    uint32_t windowFlags() override final;
    utl::Window& window() override final;
    utl::Renderer& renderer() override final;
    utl::StageManager& stageManager() override final;

    /// Decides which Stage to add to the StageManager next
    /// @param newStage Label of the string to change to.
    void trigger_stage_change(const std::string& newStage) override final;

private:
    std::string title_{constants::tetrisTitle};
    std::string version_{constants::tetrisVersion};
    std::string identifier_{constants::tetrisIdentifier};
    utl::Box screen_space_{constants::screenWidth, constants::screenHeight};
    uint32_t sdlFlags_{SDL_INIT_VIDEO};
    uint32_t windowFlags_{};
    utl::WindowWithRenderer context_;
    utl::StageManager stage_manager_;
};
