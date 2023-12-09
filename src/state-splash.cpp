// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// state-splash.cpp
//
#include "state-splash.hpp"

#include "context.hpp"
#include "game-config.hpp"
#include "layout.hpp"
#include "sdl-manager.hpp"
#include "sdl-util.hpp"
#include "state-manager.hpp"

#include <iostream>

namespace castlecrawl
{

    StateSplash::StateSplash()
        : m_castleTexturePtr(nullptr)
        , m_castleSrcRect{ 0, 0, 0, 0 }
        , m_castleDestRect{ 0, 0, 0, 0 }
        , m_lightningTexturePtr(nullptr)
        , m_lightningSrcRect{ 0, 0, 0, 0 }
        , m_lightningDestRect{ 0, 0, 0, 0 }
        , m_willShowLightning(false)
        , m_timerSec(0.0f)
        , m_fader()
    {}

    void StateSplash::onEnter(const Context & context)
    {
        const SDL_Rect screenRect = context.layout.screenRect();

        const int castleDimm = static_cast<int>(static_cast<float>(screenRect.w) * 0.3f);

        m_castleTexturePtr = context.sdl.loadAndSmoothResizeTexture(
            (context.config.media_path / "image" / "splash.png").string(),
            { castleDimm, castleDimm });

        m_castleSrcRect = util::makeRect({ 0, 0 }, util::size(m_castleTexturePtr));
        m_castleDestRect = m_castleSrcRect;

        m_castleDestRect.x = ((screenRect.w / 2) - (m_castleDestRect.w / 2));
        m_castleDestRect.y = ((screenRect.h / 2) - (m_castleDestRect.h / 2));

        //

        const int lightningDimm = static_cast<int>(static_cast<float>(screenRect.w) * 0.125f);

        m_lightningTexturePtr = context.sdl.loadAndSmoothResizeTexture(
            (context.config.media_path / "image" / "lightning.png").string(),
            { lightningDimm, lightningDimm });

        m_lightningSrcRect = util::makeRect({ 0, 0 }, util::size(m_lightningTexturePtr));
        m_lightningDestRect = m_lightningSrcRect;

        m_lightningDestRect.x = ((screenRect.w / 2) - (m_lightningDestRect.w / 2));
        m_lightningDestRect.y = ((screenRect.h / 2) - (m_lightningDestRect.h / 2));

        m_lightningDestRect.x -= m_lightningDestRect.w;
        m_lightningDestRect.y -= m_lightningDestRect.h;

        //

        m_fader.setup(true, { 0, 0, 0, 255 }, 1.5f, screenRect);
    }

    void StateSplash::onExit(const Context &)
    {
        util::destroyTexture(m_castleTexturePtr);
        util::destroyTexture(m_lightningTexturePtr);
    }

    void StateSplash::update(const Context &, const float frameTimeSec)
    {
        m_fader.update(frameTimeSec);

        //

        {
            m_timerSec += frameTimeSec;
            if ((m_timerSec >= 3.5f) && (m_timerSec < 3.75f))
            {
                m_willShowLightning = true;
                return;
            }
            else if ((m_timerSec >= 6.0f) && (m_timerSec < 6.25f))
            {
                m_willShowLightning = true;
                return;
            }
            else if ((m_timerSec >= 7.0f) && (m_timerSec < 7.25f))
            {
                m_willShowLightning = true;
                return;
            }
            else if (m_timerSec >= 9.0f)
            {
                m_timerSec = 0.0f;
            }

            m_willShowLightning = false;
        }
    }

    void StateSplash::draw(const Context & context) const
    {
        if (m_castleTexturePtr != nullptr)
        {
            context.sdl.blit(m_castleTexturePtr, m_castleSrcRect, m_castleDestRect);
        }

        if (m_willShowLightning && (m_lightningTexturePtr != nullptr))
        {
            context.sdl.blit(m_lightningTexturePtr, m_lightningSrcRect, m_lightningDestRect);
        }

        m_fader.draw(context);
    }

    void StateSplash::handleEvent(const Context & context, const SDL_Event & event)
    {
        if (m_fader.isFading())
        {
            return;
        }

        if ((event.type == SDL_KEYUP) && (event.key.keysym.sym == SDLK_e))
        {
            context.state.change(context, State::Editor);
        }
        else if ((event.type == SDL_KEYUP) || (event.type == SDL_MOUSEBUTTONDOWN))
        {
            context.state.change(context, State::Play);
        }
    }

} // namespace castlecrawl
