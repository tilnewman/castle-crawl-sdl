#ifndef MAPOBJECTS_HPP_INCLUDED
#define MAPOBJECTS_HPP_INCLUDED
//
// map-objects.hpp
//
#include "display/tile-image-enum.hpp"
#include "map/map-types.hpp"

#include <SDL.h>

#include <vector>

namespace castlecrawl
{
    struct Context;

    enum class MapObject
    {
        Bag,
        Barrel,
        Coffin,
        Chest
    };

    inline constexpr TileImage tileImage(const MapObject mapObject) noexcept
    {
        // clang-format off
        switch (mapObject)
        {
            case MapObject::Bag:    { return TileImage::Bag;    }
            case MapObject::Barrel: { return TileImage::Barrel; }
            case MapObject::Coffin: { return TileImage::Coffin; }
            case MapObject::Chest:  { return TileImage::Chest;  }
            default:                { return TileImage::Empty;  }
        }
        // clang-format on
    }

    struct MapObjectInstance
    {
        MapObjectInstance(const MapObject mapObject, const MapPos_t & pos);

        MapObject object;
        MapPos_t position;
    };

    class MapObjects
    {
      public:
        MapObjects();

        void add(const MapObjectInstance & enemy);
        void remove(const MapPos_t & position);
        void removeAll() { m_objects.clear(); }
        void draw(const Context & context) const;
        bool isAnyAtMapPos(const MapPos_t & pos) const;

      private:
        std::vector<MapObjectInstance> m_objects;
    };

} // namespace castlecrawl

#endif // MAPOBJECTS_HPP_INCLUDED
