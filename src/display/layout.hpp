#ifndef LAYOUT_HPP_INCLUDED
#define LAYOUT_HPP_INCLUDED
//
// layout.hpp
//
#include "misc/game-config.hpp"

#include <SDL.h>

namespace castlecrawl
{

    class Layout
    {
      public:
        Layout();

        const SDL_Rect screenRect() const { return m_screenRect; }
        const SDL_Rect topRect() const { return m_topRect; }
        const SDL_Rect botRect() const { return m_botRect; }
        const SDL_Rect mapRect() const { return m_mapRect; }
        const SDL_Point cellSize() const { return m_cellSize; }
        const SDL_Point cellCount() const { return m_cellCount; }

        void setup(const GameConfig & config);
        void setupNewMap(const SDL_Point & cellCount);

      private:
        SDL_Rect m_screenRect;
        SDL_Rect m_topRect;
        SDL_Rect m_botRect;
        SDL_Rect m_mapRect;
        SDL_Point m_cellSize;
        SDL_Point m_cellCount;
    };

} // namespace castlecrawl

#endif // LAYOUT_HPP_INCLUDED
