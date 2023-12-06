#ifndef FADER_HPP_INCLUDED
#define FADER_HPP_INCLUDED
//
// fader.hpp
//
#include <SDL.h>

namespace castlecrawl
{
    struct Context;

    class Fader
    {
      public:
        Fader();

        void reset();

        void setup(
            const bool isFadingIn,
            const SDL_Color & color,
            const float durationSec,
            const SDL_Rect & rect);

        // returns true only if the update transitioned from isFading() to !isFading()
        bool update(const float frameTimeSec);

        void draw(const Context & context) const;
        bool isFading() const { return (m_timerSec < m_durationSec); }
        bool isFadingIn() const { return m_isFadingIn; }

      private:
        bool m_isFadingIn;
        SDL_Rect m_rect;
        float m_timerSec;
        float m_durationSec;
        SDL_Color m_fadeColor;
        SDL_Color m_currentColor;
    };

} // namespace castlecrawl

#endif // CASTLECRAWL2_FADER_HPP_INCLUDED
