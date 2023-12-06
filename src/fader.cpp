// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// fader.cpp
//
#include "fader.hpp"

//#include "color-range.hpp"
#include "context.hpp"
#include "sdl-manager.hpp"
#include "sdl-util.hpp"

namespace castlecrawl
{

    Fader::Fader()
        : m_isFadingIn(true) // anything works here
        , m_rect{ 0, 0, 0, 0 }
        , m_timerSec(0.0f)
        , m_durationSec(0.0f)
        , m_fadeColor{ 0, 0, 0, 255 }
        , m_currentColor{ 0, 0, 0, 0 }
    {
        reset();
    }

    void Fader::reset()
    {
        m_timerSec = 0.0f;
        m_durationSec = 0.0f;
        m_rect = { 0, 0, 0, 0 };
        m_currentColor = { 0, 0, 0, 0 };
    }

    void Fader::setup(
        const bool isFadingIn,
        const SDL_Color & color,
        const float durationSec,
        const SDL_Rect & rect)
    {
        m_rect = rect;
        m_timerSec = 0.0f;
        m_fadeColor = color;
        m_isFadingIn = isFadingIn;
        m_durationSec = durationSec;

        if (m_isFadingIn)
        {
            m_currentColor = m_fadeColor;
        }
        else
        {
            m_currentColor = { 0, 0, 0, 0 };
        }
    }

    bool Fader::update(const float frameTimeSec)
    {
        const bool isFadingBefore = isFading();

        m_timerSec += frameTimeSec;
        if (m_timerSec > m_durationSec)
        {
            m_timerSec = m_durationSec;
        }

        const bool isFadingAfter = isFading();

        const float ratio(m_timerSec / m_durationSec);

        if (m_isFadingIn)
        {
            m_currentColor = util::color::blend(ratio, m_fadeColor, { 0, 0, 0, 0 });
        }
        else
        {
            m_currentColor = util::color::blend(ratio, { 0, 0, 0, 0 }, m_fadeColor);
        }

        return (isFadingBefore && !isFadingAfter);
    }

    void Fader::draw(const Context & context) const { context.sdl.draw(m_currentColor, m_rect); }

} // namespace castlecrawl
