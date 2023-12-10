// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// player.cpp
//
#include "map/player-display.hpp"

#include "display/layout.hpp"
#include "display/sdl-manager.hpp"
#include "display/tile-images.hpp"
#include "map/map.hpp"
#include "misc/check-macros.hpp"
#include "misc/context.hpp"
#include "misc/sdl-util.hpp"

namespace castlecrawl
{

    PlayerDisplay::PlayerDisplay()
        : m_mapPos{ 0, 0 }
        , m_screenPos{ 0, 0 }
        , m_shaker()
        , m_shakeTimerSec(0.0f)
        , m_willShake(false)
    {}

    void PlayerDisplay::update(const float frameTimeSec)
    {
        if (m_willShake)
        {
            m_shakeTimerSec += frameTimeSec;
            m_shaker.update(frameTimeSec);

            const float shakeDurationSec = 0.4f;
            if (m_shakeTimerSec > shakeDurationSec)
            {
                m_willShake = false;
                m_shakeTimerSec = 0.0f;
            }
        }
    }

    void PlayerDisplay::position(const Context & context, const MapPos_t & newPosition)
    {
        // assert newPosition valid? -no, because map transitions are expected to be invalid

        m_mapPos = newPosition;
        m_screenPos = context.map.mapPosToScreenPos(context, m_mapPos);
    }

    void PlayerDisplay::draw(const Context & context)
    {
        const SDL_Rect srcRect = tileImageToRect(TileImage::Avatar);
        SDL_Rect destRect = util::makeRect(m_screenPos, context.layout.cellSize());

        if (m_willShake)
        {
            destRect.x += m_shaker.adjustment();
        }

        context.sdl.blit(context.tile_image.texturePtr(), srcRect, destRect);
    }

    void PlayerDisplay::shake(const Context & context)
    {
        m_willShake = true;

        m_shaker.setup(
            static_cast<int>(static_cast<float>(context.layout.cellSize().x) * 0.1f), 75.0f);
    }

    bool PlayerDisplay::isPosNextTo(const MapPos_t pos) const
    {
        if ((m_mapPos.x == pos.x) && ((m_mapPos.y == (pos.y + 1)) || (m_mapPos.y == (pos.y - 1))))
        {
            return true;
        }
        else if (
            (m_mapPos.y == pos.y) && ((m_mapPos.x == (pos.x + 1)) || (m_mapPos.x == (pos.x - 1))))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

} // namespace castlecrawl
