#ifndef PLAYERDISPLAY_HPP_INCLUDED
#define PLAYERDISPLAY_HPP_INCLUDED
//
// player-display.hpp
//
#include "display/shaker.hpp"
#include "map/map-types.hpp"

namespace castlecrawl
{
    struct Context;

    class PlayerDisplay
    {
      public:
        PlayerDisplay();

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

#endif // PLAYERDISPLAY_HPP_INCLUDED
