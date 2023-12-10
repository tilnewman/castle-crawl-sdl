#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED
//
// map.hpp
//
#include "map/map-types.hpp"
#include "map/quick-map.hpp"
#include "misc/game-config.hpp"

#include <string>
#include <vector>

#include <SDL.h>

namespace castlecrawl
{
    struct Context;

    class Map
    {
      public:
        Map();

        Map(const MapName name,
            const Context & context,
            const Floor floor,
            const MapChars_t & mapChars,
            const MapTransitions_t & transVec);

        MapName name() const { return m_name; }
        Floor floor() const { return m_floor; }
        const MapTransitions_t transitions() const { return m_transitions; }
        bool isEmpty() const { return m_map.empty(); }
        const SDL_Point size() const;
        bool isPosValid(const MapPos_t & pos) const;
        const MapCell cell(const MapPos_t & pos) const;
        void setObjectChar(const MapPos_t & pos, const char newChar);

        const MapPos_t
            screenPosToMapPos(const Context & context, const SDL_Point & screenPos) const;

        const SDL_Point mapPosToScreenPos(const Context & context, const MapPos_t & mapPos) const;

        const std::vector<MapCell> surroundingCellsHorizVert(const MapPos_t & pos) const;
        const std::vector<MapCell> surroundingCellsAll(const MapPos_t & pos) const;

        void dumpToConsole() const;

      private:
        MapName m_name;
        Map_t m_map;
        Floor m_floor;
        MapTransitions_t m_transitions;
    };

} // namespace castlecrawl

#endif // MAP_HPP_INCLUDED
