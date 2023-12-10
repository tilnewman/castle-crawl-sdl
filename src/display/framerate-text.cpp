// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// framerate-text.cpp
//
#include "display/framerate-text.hpp"

#include "misc/context.hpp"
#include "display/font.hpp"
#include "display/layout.hpp"
#include "display/sdl-manager.hpp"
#include "misc/sdl-util.hpp"

#include <string>

#include <SFML/Graphics/Text.hpp>

namespace castlecrawl
{
    FramerateText::FramerateText()
        : m_frameCounter(0.0f)
        , m_elapsedSec(0.0f)
        , m_texturePtr(nullptr)
        , m_screenPos{ 0, 0 }
    {}

    void FramerateText::setup() {}

    void FramerateText::teardown() { util::destroyTexture(m_texturePtr); }

    void FramerateText::update(const Context & context, const float frameTimeSec)
    {
        m_frameCounter += 1.0f;

        m_elapsedSec += frameTimeSec;
        if (m_elapsedSec >= 1.0f)
        {
            const std::size_t fps = static_cast<std::size_t>(m_frameCounter / m_elapsedSec);
            const std::string fpsString = std::to_string(fps) + "fps";

            util::destroyTexture(m_texturePtr);

            m_texturePtr = context.font.makeTexture(context, FontSize::Medium, fpsString);

            m_screenPos.x = 0;
            m_screenPos.y = ((context.layout.botRect().y + context.layout.botRect().h) - 50);

            m_frameCounter = 0.0f;
            m_elapsedSec = 0.0f;
        }
    }

    void FramerateText::draw(const Context & context) const
    {
        if (m_texturePtr != nullptr)
        {
            context.sdl.blit(m_texturePtr, m_screenPos.x, m_screenPos.y);
        }
    }

} // namespace castlecrawl
