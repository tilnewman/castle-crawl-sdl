#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED
//
// player.hpp
//
#include "map-types.hpp"
#include "shaker.hpp"

namespace castlecrawl
{
    struct Context;

    class Player
    {
      public:
        Player();

        void update(const float frameTimeSec);
        const MapPos_t position() const { return m_mapPos; }
        void position(const Context & context, const MapPos_t & newPosition);
        void draw(const Context & context);
        void shake(const Context & context);
        bool isPosNextTo(const MapPos_t pos) const;

      private:
        MapPos_t m_mapPos;
        SDL_Point m_screenPos;
        Shaker m_shaker;
        float m_shakeTimerSec;
        bool m_willShake;
    };

} // namespace castlecrawl

#endif // PLAYER_HPP_INCLUDED
