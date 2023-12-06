#ifndef MOUSEOVER_HPP_INCLUDED
#define MOUSEOVER_HPP_INCLUDED
//
// mouseover.hpp
//
#include <SDL.h>

namespace castlecrawl
{
    struct Context;

    class Mouseover
    {
      public:
        Mouseover();

        void setup(const Context & context);
        void teardown();
        void reset();
        void update(const Context & context, const float frameTimeSec);
        void handleMouseMovedEvent(const SDL_Point & newPos);
        void draw(const Context & context) const;

      private:
        void setupDisplay(const Context & context);

      private:
        float m_elapsedTimeSec;
        bool m_willDisplay;
        SDL_Point m_mousePos;
        SDL_Texture * m_textTexturePtr;
        SDL_Point m_textScreenPos;
    };

} // namespace castlecrawl

#endif // MOUSEOVER_HPP_INCLUDED
