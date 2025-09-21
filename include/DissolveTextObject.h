#pragma once

#include "constants.h"
#include <chrono>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>
#include <utl_TextObject.hpp>

class DissolveTextObject : public utl::TextObject {
public:
    DissolveTextObject() = default;
    DissolveTextObject(utl::Stage* stage, utl::Font* font,
                       const utl::Colour& colour, const std::string& newText);
    DissolveTextObject(utl::Stage* stage, utl::Font* font,
                       const utl::Colour& colour, const utl::Vec2d& pos);
    DissolveTextObject(utl::Stage* stage, utl::Font* font,
                       const utl::Colour& colour, const std::string& newText,
                       const utl::Vec2d& pos);
    DissolveTextObject(utl::Stage* stage, utl::Font* font,
                       const utl::Colour& colour, const std::string& newText,
                       const utl::Vec2d& pos,
                       std::chrono::milliseconds flashLength);

    void update(std::chrono::milliseconds t,
                std::chrono::milliseconds dt) override;
    void render(utl::Renderer& renderer) override;

    void switchOn();

    std::chrono::milliseconds visibleTime{
        constants::dissolveTODefaultVisibleTime};

private:
    bool isVisible_{false};
    std::chrono::milliseconds visibleTimer{0};
    utl::Rect mask;
    utl::Colour maskColour{};
};
