#ifndef TOPPANEL_HPP_INCLUDED
#define TOPPANEL_HPP_INCLUDED
//
// top-panel.hpp
//
#include <SDL.h>

namespace castlecrawl
{
    struct Context;

    class TopPanel
    {
      public:
        TopPanel();

        void setup(const Context & context);
        void teardown();
        void draw(const Context & context) const;

      private:
        SDL_Texture * m_titleTexturePtr;
        SDL_Rect m_titleDestRect;
        SDL_Rect m_titleSrcRect;
    };

} // namespace castlecrawl

#endif // TOPPANEL_HPP_INCLUDED
