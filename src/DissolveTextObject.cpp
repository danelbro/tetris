#include "DissolveTextObject.h"

#include "colours.h"
#include "constants.h"

#include <chrono>
#include <cstdint>
#include <utl_SDLInterface.hpp>
#include <utl_Stage.hpp>
#include <utl_TextObject.hpp>

using namespace std::chrono_literals;

static auto setColourAlphaPercent(const utl::Colour& colour, double alphaPercent)
    -> utl::Colour;

DissolveTextObject::DissolveTextObject(utl::Stage* stage, utl::Font* font,
                                       const utl::Colour& colour,
                                       const std::string& newText)
    : DissolveTextObject{stage,   font, colour,
                         newText, {},   constants::dissolveTODefaultVisibleTime}
{}

DissolveTextObject::DissolveTextObject(utl::Stage* stage, utl::Font* font,
                                       const utl::Colour& colour,
                                       const utl::Vec2d& pos)
    : DissolveTextObject{stage, font, colour,
                         {},    pos,  constants::dissolveTODefaultVisibleTime}
{}

DissolveTextObject::DissolveTextObject(utl::Stage* stage, utl::Font* font,
                                       const utl::Colour& colour,
                                       const std::string& newText,
                                       const utl::Vec2d& pos)
    : DissolveTextObject{stage,   font, colour,
                         newText, pos,  constants::dissolveTODefaultVisibleTime}
{}

DissolveTextObject::DissolveTextObject(utl::Stage* stage, utl::Font* font,
                                       const utl::Colour& colour,
                                       const std::string& newText,
                                       const utl::Vec2d& pos,
                                       std::chrono::milliseconds flashLength)
    : utl::TextObject{stage, font, colour, newText, pos},
      visibleTime{flashLength},
      mask{{static_cast<float>(m_pos.x), static_cast<float>(m_pos.y),
            static_cast<float>(m_size.w), static_cast<float>(m_size.h)}},
      maskColour{colours::background}
{}

void DissolveTextObject::update(std::chrono::milliseconds,
                                std::chrono::milliseconds dt)
{
    if (!isVisible_)
        return;

    mask.reset({static_cast<float>(m_pos.x), static_cast<float>(m_pos.y),
                static_cast<float>(m_size.w), static_cast<float>(m_size.h)});
    visibleTimer += dt;

    if (visibleTimer < visibleTime) {
        auto alphaPercent = static_cast<double>(visibleTimer.count())
                            / static_cast<double>(visibleTime.count());
        maskColour = setColourAlphaPercent(colours::background, alphaPercent);
    }

    else {
        using namespace std::chrono_literals;
        isVisible_ = false;
        visibleTimer = 0ms;
    }
}

void DissolveTextObject::render(utl::Renderer& renderer)
{
    if (isVisible_) {
        utl::TextObject::render(renderer);
        auto oldCol = utl::getRendererDrawColour(renderer);
        utl::setRendererDrawColour(renderer, maskColour);
        mask.draw(renderer);
        utl::setRendererDrawColour(renderer, oldCol);
    }
}

void DissolveTextObject::switchOn()
{
    isVisible_ = true;
    maskColour.a = 0x00;
}

static auto setColourAlphaPercent(const utl::Colour& colour, double alphaPercent)
    -> utl::Colour
{
    auto newCol = colour;
    newCol.a = static_cast<uint8_t>(static_cast<double>(colour.a) * alphaPercent);
    return newCol;
}
