// -*- C++ -*- //
/// @file An application that plays a tetris clone

#pragma once

#include <string>
#include <utl_Application.hpp>
#include <utl_StageManager.hpp>
#include <utl_utility.hpp>

class TetrisApp : public utl::Application {
public:
    /// Initialises the Application and adds a TitleScreen Stage to the
    /// StageManager.
    ///
    /// @param title The title of the application
    /// @param screenWidth The width of the screen
    /// @param screenHeight The height of the screen
    /// @param windowFlags Alias for SDL_WindowFlags
    TetrisApp(const std::string& title, int screenWidth, int screenHeight,
              unsigned windowFlags);
    ~TetrisApp();

    /// Starts the StageManager state machine.
    void run() override;

    /// Decides which Stage to add to the StageMnager next
    ///
    /// @param newStage Label of the string to change to.
    void trigger_stage_change(const std::string& newStage) override;
};
