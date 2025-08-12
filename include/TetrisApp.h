// -*- C++ -*- //
/// @file An application that plays a tetris clone

#pragma once

#include <string>
#include <utl_Application.hpp>
#include <utl_StageManager.hpp>

class TetrisApp : public utl::Application {
public:
    /// Initialises the Application and adds a TitleScreen Stage to the
    /// StageManager.
    TetrisApp();
    ~TetrisApp();

    /// Decides which Stage to add to the StageManager next
    /// @param newStage Label of the string to change to.
    void trigger_stage_change(const std::string& newStage) override;
};
