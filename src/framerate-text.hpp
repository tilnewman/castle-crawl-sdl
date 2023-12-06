#ifndef FRAMERATETEXT_HPP_INCLUDED
#define FRAMERATETEXT_HPP_INCLUDED
//
// framerate-text.hpp
//
#include <SDL.h>

namespace castlecrawl
{
    struct Context;

    class FramerateText
    {
      public:
        FramerateText();

        void setup();
        void teardown();
        void update(const Context & context, const float frameTimeSec);
        void draw(const Context & context) const;

      private:
        float m_frameCounter;
        float m_elapsedSec;
        SDL_Texture * m_texturePtr;
        SDL_Point m_screenPos;
    };

} // namespace castlecrawl

#endif // CASTLECRAWL2_FRAMERATETEXT_HPP_INCLUDED
