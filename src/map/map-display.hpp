#ifndef MAPDISPLAY_HPP_INCLUDED
#define MAPDISPLAY_HPP_INCLUDED
//
// map-display.hpp
//
#include <SDL.h>

#include "map/map.hpp"

namespace castlecrawl
{
    struct Context;

    class MapDisplay
    {
      public:
        MapDisplay();

        void load(const Context & context);
        void dumpToConsole() const;
        void draw(const Context & context);

      private:
        void reset();
        void appendBlits(const Context & context);
        void appendLiquidEdgeRects(const Context & context);

      private:
        BlitRectsVec_t m_objectBlits;
        BlitRectsVec_t m_floorBlits;
        RectVec_t m_floorEdgeRects;
    };

} // namespace castlecrawl

#endif // MAPDISPLAY_HPP_INCLUDED
