#ifndef QUICKMAP_HPP_INCLUDED
#define QUICKMAP_HPP_INCLUDED
//
// quick-map.hpp
//
#include "game-config.hpp"
#include "map-types.hpp"

namespace castlecrawl
{
    // QuickMaps are the maps found in the source code in quick-maps.hpp, that are temporary
    // placeholders for real maps that are easy for humans to manage and are easily convertible
    // into real maps.
    class QuickMap
    {
      public:
        QuickMap(const GameConfig & config, const MapChars_t & mapChars);

        bool empty() const { return m_mapChars.empty(); }

        // assumes all rows are the same length, see convertQuickMapToRealMap_VerifySize()
        const SDL_Point size() const
        {
            return SDL_Point{ static_cast<int>(m_mapChars.front().size()),
                              static_cast<int>(m_mapChars.size()) };
        }

        bool isPosValid(const MapPos_t & pos) const
        {
            return (
                (pos.x >= 0) && (pos.y >= 0) && (pos.x < static_cast<int>(size().x)) &&
                (pos.y < static_cast<int>(size().y)));
        }

        char getChar(const int x, const int y) const;

        void setChar(const int x, const int y, const char ch)
        {
            m_mapChars.at(static_cast<std::size_t>(y)).at(static_cast<std::size_t>(x)) = ch;
        }

        void dumpToConsole() const;

      private:
        void convertQuickMapToRealMap(const GameConfig & config);
        void convertQuickMapToRealMap_VerifySize(const GameConfig & config) const;
        void convertQuickMapToRealMap_Walls();
        void convertQuickMapToRealMap_WallCorners();

      private:
        MapChars_t m_mapChars;
    };

} // namespace castlecrawl

#endif // QUICKMAP_HPP_INCLUDED
