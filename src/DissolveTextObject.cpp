#include "DissolveTextObject.h"
#include "utl_SDLInterface.hpp"
#include "utl_TextObject.hpp"

#include <chrono>
#include <utl_Stage.hpp>

using namespace std::chrono_literals;

DissolveTextObject::DissolveTextObject(utl::Stage* stage, utl::Font* font,
                                       const utl::Colour& colour,
                                       const std::string& newText)
    : DissolveTextObject{stage, font, colour, newText, {}, 5000ms}
{}

DissolveTextObject::DissolveTextObject(utl::Stage* stage, utl::Font* font,
                                       const utl::Colour& colour,
                                       const utl::Vec2d& pos)
    : DissolveTextObject{stage, font, colour, {}, pos, 5000ms}
{}

DissolveTextObject::DissolveTextObject(utl::Stage* stage, utl::Font* font,
                                       const utl::Colour& colour,
                                       const std::string& newText,
                                       const utl::Vec2d& pos)
    : DissolveTextObject{stage, font, colour, newText, pos, 5000ms}
{}

DissolveTextObject::DissolveTextObject(utl::Stage* stage, utl::Font* font,
                                       const utl::Colour& colour,
                                       const std::string& newText,
                                       const utl::Vec2d& pos,
                                       std::chrono::milliseconds flashLength)
    : utl::TextObject{stage, font, colour, newText, pos},
      visibleTime{flashLength}
{}

void DissolveTextObject::update(std::chrono::milliseconds,
                                std::chrono::milliseconds dt)
{
    using namespace std::chrono_literals;
    if (renderMe_)
        visibleTimer += dt;

    if (visibleTimer >= visibleTime) {
        renderMe_ = false;
        visibleTimer = 0ms;
    }
}

void DissolveTextObject::render(utl::Renderer& renderer)
{
    if (renderMe_)
        utl::TextObject::render(renderer);
}

void DissolveTextObject::renderMe()
{
    renderMe_ = true;
}
