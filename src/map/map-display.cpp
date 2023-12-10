// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// map-display.cpp
//
#include "map/map-display.hpp"

#include "display/layout.hpp"
#include "display/sdl-manager.hpp"
#include "display/tile-image-enum.hpp"
#include "display/tile-images.hpp"
#include "misc/context.hpp"
#include "misc/sdl-util.hpp"

#include <iostream>

namespace castlecrawl
{

    MapDisplay::MapDisplay()
        : m_objectBlits()
        , m_floorBlits()
        , m_floorEdgeRects()
    {}

    void MapDisplay::load(const Context & context)
    {
        reset();
        appendBlits(context);
        appendLiquidEdgeRects(context);
        // dumpToConsole();
    }

    void MapDisplay::reset()
    {
        m_objectBlits.clear();
        m_floorBlits.clear();
        m_floorEdgeRects.clear();

        // these values found by experiment to work well with even the largest of maps
        m_objectBlits.reserve(2000);
        m_floorBlits.reserve(2000);
        m_floorEdgeRects.reserve(2000);
    }

    void MapDisplay::dumpToConsole() const
    {
        std::cout << "object_blits: " << m_objectBlits.size() << std::endl;
        std::cout << "floor_blits: " << m_floorBlits.size() << std::endl;
        std::cout << "floor_edge_rects: " << m_floorEdgeRects.size() << std::endl << std::endl;
    }

    void MapDisplay::appendBlits(const Context & context)
    {
        // make edge verts bigger to cover corner cases (it's drawn below walls anyway)
        SDL_Rect edgeRect = util::makeRect({ 0, 0 }, context.layout.cellSize());
        const float growScale = 0.5f;
        util::scaleRectInPlace(edgeRect, (1.0f + growScale));

        const int overlap{ static_cast<int>(
            static_cast<float>(context.layout.cellSize().x) * (growScale * 0.5f)) };

        char prevObjectChar('.'); // anything except '-' works here

        const SDL_Point mapSize = context.map.size();
        SDL_Point screenPos = util::position(context.layout.mapRect());
        for (int y(0); y < mapSize.y; ++y)
        {
            for (int x(0); x < mapSize.x; ++x)
            {
                const MapCell cell = context.map.cell({ x, y });

                const SDL_Rect destRect =
                    util::makeRect({ screenPos.x, screenPos.y }, context.layout.cellSize());

                if (' ' != cell.floor_char)
                {
                    const SDL_Rect srcRect = tileImageToRect(charToTileImage(cell.floor_char));
                    m_floorBlits.push_back({ srcRect, destRect });
                }

                if ('.' == cell.object_char)
                {
                    edgeRect.x = (destRect.x - overlap);
                    edgeRect.y = (destRect.y - overlap);

                    m_floorEdgeRects.push_back(edgeRect);
                }

                const TileImage objectTileImage = charToTileImage(cell.object_char);
                if (objectTileImage != TileImage::Empty)
                {
                    const SDL_Rect srcRect = tileImageToRect(objectTileImage);
                    m_objectBlits.push_back({ srcRect, destRect });
                }

                if (('-' == cell.object_char) && ('-' != prevObjectChar))
                {
                    const SDL_Rect srcRect = tileImageToRect(TileImage::Wall_Shadow);
                    m_objectBlits.push_back({ srcRect, destRect });
                }

                prevObjectChar = cell.object_char;
                screenPos.x += context.layout.cellSize().x;
            }

            screenPos.x = context.layout.mapRect().x;
            screenPos.y += context.layout.cellSize().y;
        }
    }

    void MapDisplay::appendLiquidEdgeRects(const Context & context)
    {
        auto notLiq = [](const char ch) { return ((ch != 'l') && (ch != 'w') && (ch != 'g')); };

        auto validNotLiquid = [&](const char ch, const MapPos_t & pos) {
            return (notLiq(ch) && context.map.isPosValid(pos));
        };

        auto getChar = [&](const int x, const int y) {
            const MapCell cell = context.map.cell({ x, y });
            return cell.object_char;
        };

        const SDL_Point mapSize = context.map.size();
        SDL_Point screenPos = util::position(context.layout.mapRect());
        for (int y(0); y < mapSize.y; ++y)
        {
            for (int x(0); x < mapSize.x; ++x)
            {
                const char ch{ getChar(x, y) };

                if (notLiq(ch))
                {
                    screenPos.x += context.layout.cellSize().x;
                    continue;
                }

                // chars in all directions
                const char upChar{ getChar(x, y - 1) };
                const char downChar{ getChar(x, y + 1) };
                const char leftChar{ getChar(x - 1, y) };
                const char rightChar{ getChar(x + 1, y) };

                // pos in all directions
                const MapPos_t upPos{ x, y - 1 };
                const MapPos_t downPos{ x, y + 1 };
                const MapPos_t leftPos{ x - 1, y };
                const MapPos_t rightPos{ x + 1, y };

                const SDL_Rect destRect =
                    util::makeRect({ screenPos.x, screenPos.y }, context.layout.cellSize());

                if (validNotLiquid(upChar, upPos) && validNotLiquid(leftChar, leftPos))
                {
                    const SDL_Rect srcRect = tileImageToRect(TileImage::LiquidRim_TopLeft);
                    m_objectBlits.push_back({ srcRect, destRect });
                }

                if (validNotLiquid(upChar, upPos) && validNotLiquid(rightChar, rightPos))
                {
                    const SDL_Rect srcRect = tileImageToRect(TileImage::LiquidRim_TopRight);
                    m_objectBlits.push_back({ srcRect, destRect });
                }

                if (validNotLiquid(downChar, downPos) && validNotLiquid(leftChar, leftPos))
                {
                    const SDL_Rect srcRect = tileImageToRect(TileImage::LiquidRim_BotLeft);
                    m_objectBlits.push_back({ srcRect, destRect });
                }

                if (validNotLiquid(downChar, downPos) && validNotLiquid(rightChar, rightPos))
                {
                    const SDL_Rect srcRect = tileImageToRect(TileImage::LiquidRim_BotRight);
                    m_objectBlits.push_back({ srcRect, destRect });
                }

                if (validNotLiquid(upChar, upPos))
                {
                    const SDL_Rect srcRect = tileImageToRect(TileImage::LiquidRim_Top);
                    m_objectBlits.push_back({ srcRect, destRect });
                }

                if (validNotLiquid(downChar, downPos))
                {
                    const SDL_Rect srcRect = tileImageToRect(TileImage::LiquidRim_Bot);
                    m_objectBlits.push_back({ srcRect, destRect });
                }

                if (validNotLiquid(leftChar, leftPos))
                {
                    const SDL_Rect srcRect = tileImageToRect(TileImage::LiquidRim_Left);
                    m_objectBlits.push_back({ srcRect, destRect });
                }

                if (validNotLiquid(rightChar, rightPos))
                {
                    const SDL_Rect srcRect = tileImageToRect(TileImage::LiquidRim_Right);
                    m_objectBlits.push_back({ srcRect, destRect });
                }

                screenPos.x += context.layout.cellSize().x;
            }

            screenPos.x = context.layout.mapRect().x;
            screenPos.y += context.layout.cellSize().x;
        }
    }

    void MapDisplay::draw(const Context & context)
    {
        context.sdl.blit(context.tile_image.texturePtr(), m_floorBlits);
        context.sdl.draw(context.config.background_color, m_floorEdgeRects);
        context.sdl.blit(context.tile_image.texturePtr(), m_objectBlits);
    }

} // namespace castlecrawl
