#include "TetrisApp.h"

#include <string>
#include <utl_Application.hpp>
#include <utl_utility.hpp>

TetrisApp::TetrisApp(const std::string& title, int screenWidth,
                     int screenHeight, unsigned windowFlags)
    : utl::Application{title.c_str(), screenWidth, screenHeight, windowFlags}
{
    LOG("Created TetrisApp!\n");
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
