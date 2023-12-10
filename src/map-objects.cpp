// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// map-objects.cpp
//
#include "map-objects.hpp"

#include "check-macros.hpp"
#include "context.hpp"
#include "layout.hpp"
#include "map.hpp"
#include "player-display.hpp"
#include "sdl-manager.hpp"
#include "sdl-util.hpp"
#include "tile-images.hpp"

namespace castlecrawl
{

    MapObjectInstance::MapObjectInstance(const MapObject mapObject, const MapPos_t & pos)
        : object(mapObject)
        , position(pos)
    {}

    MapObjects::MapObjects()
        : m_objects()
    {}

    void MapObjects::add(const MapObjectInstance & moi) { m_objects.push_back(moi); }

    void MapObjects::remove(const MapPos_t & pos)
    {
        m_objects.erase(
            std::remove_if(
                std::begin(m_objects),
                std::end(m_objects),
                [&](const MapObjectInstance & moi) { return (moi.position == pos); }),
            std::end(m_objects));
    }

    void MapObjects::draw(const Context & context) const
    {
        for (const MapObjectInstance & moi : m_objects)
        {
            const SDL_Rect srcRect = tileImageToRect(tileImage(moi.object));

            const SDL_Rect destRect = util::makeRect(
                context.map.mapPosToScreenPos(context, moi.position), context.layout.cellSize());

            context.sdl.blit(context.tile_image.texturePtr(), srcRect, destRect);
        }
    }

    bool MapObjects::isAnyAtMapPos(const MapPos_t & pos) const
    {
        for (const MapObjectInstance & moi : m_objects)
        {
            if (moi.position == pos)
            {
                return true;
            }
        }

        return false;
    }

} // namespace castlecrawl
