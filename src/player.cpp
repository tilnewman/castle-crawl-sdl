// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// player.cpp
//
#include "player.hpp"

#include "check-macros.hpp"
#include "context.hpp"
#include "layout.hpp"
#include "map.hpp"
#include "sdl-manager.hpp"
#include "sdl-util.hpp"
#include "tile-images.hpp"

namespace castlecrawl
{

    Player::Player()
        : m_mapPos{ 0, 0 }
        , m_screenPos{ 0, 0 }
        , m_shaker()
        , m_shakeTimerSec(0.0f)
        , m_willShake(false)
    {}

    void Player::update(const float frameTimeSec)
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

    void Player::position(const Context & context, const MapPos_t & newPosition)
    {
        // assert newPosition valid? -no, because map transitions are expected to be invalid

        m_mapPos = newPosition;
        m_screenPos = context.map.mapPosToScreenPos(context, m_mapPos);
    }

    void Player::draw(const Context & context)
    {
        const SDL_Rect srcRect = tileImageToRect(TileImage::Avatar);
        SDL_Rect destRect = util::makeRect(m_screenPos, context.layout.cellSize());

        if (m_willShake)
        {
            destRect.x += m_shaker.adjustment();
        }

        context.sdl.blit(context.tile_image.texturePtr(), srcRect, destRect);
    }

    void Player::shake(const Context & context)
    {
        m_willShake = true;

        m_shaker.setup(
            static_cast<int>(static_cast<float>(context.layout.cellSize().x) * 0.1f), 75.0f);
    }

    bool Player::isPosNextTo(const MapPos_t pos) const
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
