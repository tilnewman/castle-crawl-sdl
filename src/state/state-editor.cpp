// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// state-editor.cpp
//
#include "state/state-editor.hpp"

#include "display/font.hpp"
#include "display/layout.hpp"
#include "display/sdl-manager.hpp"
#include "map/map-display.hpp"
#include "map/map.hpp"
#include "misc/context.hpp"
#include "misc/game-config.hpp"
#include "misc/sdl-util.hpp"
#include "misc/sound-player.hpp"

#include <fstream>
#include <iostream>

#include <SFML/Graphics/Text.hpp>

namespace castlecrawl
{

    StateEditor::StateEditor()
        : m_mapChars()
        , m_editPos{ 0, 0 }
        , m_floor(Floor::Stone)
        , m_editRect{ 0, 0, 0, 0 }
        , m_keyTexturePtr(nullptr)
        , m_keyScreenPos{ 0, 0 }
        , m_fadeTexturePtr(nullptr)
        , m_fadeScreenPos{ 0, 0 }
        , m_titleTexturePtr(nullptr)
        , m_titleScreenPos{ 0, 0 }
        , m_fadeTextAlpha(0)
        , m_isDragging(false)
        , m_dragPosStart{ 0, 0 }
        , m_dragPosStop{ 0, 0 }
        , m_dragRect{ 0, 0, 0, 0 }
        , m_dragSelectedEntrys()
    {}

    void StateEditor::onEnter(const Context & context)
    {
        m_mapChars = std::vector<std::string>(
            static_cast<std::size_t>(context.config.map_cell_size_max.y),
            std::string(static_cast<std::size_t>(context.config.map_cell_size_max.x), '.'));

        resetMap(context);

        m_editRect =
            util::makeRect(util::position(context.layout.mapRect()), context.layout.cellSize());

        m_titleTexturePtr = context.font.makeTexture(
            context, FontSize::Medium, "Press and hold 'h' for help.", { 127, 127, 127, 255 });

        m_titleScreenPos =
            (util::center(context.layout.topRect()) - (util::size(m_titleTexturePtr) / 2));
    }

    void StateEditor::onExit(const Context &)
    {
        util::destroyTexture(m_keyTexturePtr);
        util::destroyTexture(m_fadeTexturePtr);
        util::destroyTexture(m_titleTexturePtr);
    }

    void StateEditor::update(const Context &, const float)
    {
        if (m_fadeTextAlpha > 0)
        {
            --m_fadeTextAlpha;
            SDL_SetTextureAlphaMod(m_fadeTexturePtr, m_fadeTextAlpha);
        }
        else
        {
            util::destroyTexture(m_fadeTexturePtr);
        }
    }

    void StateEditor::draw(const Context & context) const
    {
        context.map_display.draw(context);
        context.sdl.drawOutline({ 255, 255, 255, 64 }, context.layout.mapRect());

        if (m_dragSelectedEntrys.empty())
        {
            context.sdl.draw({ 0, 255, 255, 64 }, m_editRect);
        }

        if (m_keyTexturePtr != nullptr)
        {
            context.sdl.blit(m_keyTexturePtr, m_keyScreenPos.x, m_keyScreenPos.y);
        }

        if (m_fadeTexturePtr != nullptr)
        {
            context.sdl.blit(m_fadeTexturePtr, m_fadeScreenPos.x, m_fadeScreenPos.y);
        }

        if (m_titleTexturePtr != nullptr)
        {
            context.sdl.blit(m_titleTexturePtr, m_titleScreenPos.x, m_titleScreenPos.y);
        }

        if (m_isDragging)
        {
            context.sdl.draw({ 255, 255, 255, 32 }, m_dragRect);
        }

        for (const MapEntry_t & entry : m_dragSelectedEntrys)
        {
            context.sdl.draw({ 0, 255, 255, 64 }, entry.rect);
        }
    }

    void StateEditor::handleEvent(const Context & context, const SDL_Event & event)
    {
        if ((event.type == SDL_MOUSEBUTTONDOWN) && (event.button.button == SDL_BUTTON_LEFT))
        {
            startDragging(context, { event.button.x, event.button.y });
            return;
        }

        if ((event.type == SDL_MOUSEBUTTONUP) && (event.button.button == SDL_BUTTON_LEFT))
        {
            stopDragging(context, { event.button.x, event.button.y });
            return;
        }

        if (m_isDragging && (event.type == SDL_MOUSEMOTION))
        {
            updateDragging(context, { event.button.x, event.button.y });
            return;
        }

        if ((SDL_KEYDOWN == event.type) && (SDLK_h == event.key.keysym.sym))
        {
            setupKeyText(context);
            return;
        }
        else if ((SDL_KEYUP == event.type) && (SDLK_h == event.key.keysym.sym))
        {
            util::destroyTexture(m_keyTexturePtr);
            return;
        }

        // all events are key released events that only work if not dragging
        if (m_isDragging || (SDL_KEYUP != event.type))
        {
            return;
        }

        const bool isShift = (event.key.keysym.mod & KMOD_SHIFT);
        const bool isCntrl = (event.key.keysym.mod & KMOD_CTRL);

        if ((SDLK_s == event.key.keysym.sym) && isCntrl)
        {
            fadeText(context, "Saving");
            save();
        }
        else if (SDLK_f == event.key.keysym.sym)
        {
            if (m_floor == Floor::Dirt)
            {
                m_floor = Floor::Stone;
                fadeText(context, "Stone Floor");
            }
            else if (m_floor == Floor::Stone)
            {
                m_floor = Floor::Wood;
                fadeText(context, "Wood Floor");
            }
            else if (m_floor == Floor::Wood)
            {
                m_floor = Floor::Dirt;
                fadeText(context, "Dirt Floor");
            }

            resetMap(context);
            context.sfx.play("tick-off-2");
        }
        else if (event.key.keysym.sym == SDLK_UP)
        {
            if (m_editPos.y > 0)
            {
                --m_editPos.y;
                placeEditCursor(context);
                m_dragSelectedEntrys.clear();
                context.sfx.play("tick-off-1");
            }
            else
            {
                context.sfx.play("tap-wood-low");
            }
        }
        else if (event.key.keysym.sym == SDLK_DOWN)
        {
            if (m_editPos.y < (static_cast<int>(m_mapChars.size()) - 1))
            {
                ++m_editPos.y;
                placeEditCursor(context);
                m_dragSelectedEntrys.clear();
                context.sfx.play("tick-off-1");
            }
            else
            {
                context.sfx.play("tap-wood-low");
            }
        }
        else if (event.key.keysym.sym == SDLK_LEFT)
        {
            if (m_editPos.x > 0)
            {
                --m_editPos.x;
                placeEditCursor(context);
                m_dragSelectedEntrys.clear();
                context.sfx.play("tick-off-1");
            }
            else
            {
                context.sfx.play("tap-wood-low");
            }
        }
        else if (event.key.keysym.sym == SDLK_RIGHT)
        {
            if (m_editPos.x < (static_cast<int>(m_mapChars.front().size()) - 1))
            {
                ++m_editPos.x;
                placeEditCursor(context);
                m_dragSelectedEntrys.clear();
                context.sfx.play("tick-off-1");
            }
            else
            {
                context.sfx.play("tap-wood-low");
            }
        }
        // clang-format off
        else if (event.key.keysym.sym == SDLK_SPACE)        { editMap(context, isShift, ' ', ' '); }
        else if (event.key.keysym.sym == SDLK_PERIOD)       { editMap(context, isShift, '.', '.'); }
        else if (event.key.keysym.sym == SDLK_d)            { editMap(context, isShift, 'D', 'd'); }
        else if (event.key.keysym.sym == SDLK_s)            { editMap(context, isShift, 'S', 's'); }
        else if (event.key.keysym.sym == SDLK_r)            { editMap(context, isShift, 'r', 'r'); }
        else if (event.key.keysym.sym == SDLK_a)            { editMap(context, isShift, 'a', 'a'); }
        else if (event.key.keysym.sym == SDLK_l)            { editMap(context, isShift, 'l', 'l'); }
        else if (event.key.keysym.sym == SDLK_w)            { editMap(context, isShift, 'w', 'w'); }
        else if (event.key.keysym.sym == SDLK_g)            { editMap(context, isShift, 'g', 'g'); }
        else if (event.key.keysym.sym == SDLK_b)            { editMap(context, isShift, 'b', 'b'); }
        else if (event.key.keysym.sym == SDLK_c)            { editMap(context, isShift, 'c', 'c'); }
        else if (event.key.keysym.sym == SDLK_k)            { editMap(context, isShift, 'k', 'k'); }
        else if (event.key.keysym.sym == SDLK_0)            { editMap(context, isShift, '0', '0'); }
        else if (event.key.keysym.sym == SDLK_1)            { editMap(context, isShift, '1', '1'); }
        else if (event.key.keysym.sym == SDLK_2)            { editMap(context, isShift, '2', '2'); }
        else if (event.key.keysym.sym == SDLK_3)            { editMap(context, isShift, '3', '3'); }
        else if (event.key.keysym.sym == SDLK_4)            { editMap(context, isShift, '4', '4'); }
        else if (event.key.keysym.sym == SDLK_5)            { editMap(context, isShift, '5', '5'); }
        else if (event.key.keysym.sym == SDLK_6)            { editMap(context, isShift, '6', '6'); }
        else if (event.key.keysym.sym == SDLK_7)            { editMap(context, isShift, '7', '7'); }
        else if (event.key.keysym.sym == SDLK_8)            { editMap(context, isShift, '8', '8'); }
        else if (event.key.keysym.sym == SDLK_9)            { editMap(context, isShift, '9', '9'); }
        else if (event.key.keysym.sym == SDLK_SEMICOLON)    { editMap(context, isShift, ';', ':'); }
        else if (event.key.keysym.sym == SDLK_LEFTBRACKET)  { editMap(context, isShift, '[', '['); }
        else if (event.key.keysym.sym == SDLK_RIGHTBRACKET) { editMap(context, isShift, ']', ']'); }
        // clang-format on
    }

    void StateEditor::startDragging(const Context & context, const SDL_Point & mousePos)
    {
        m_dragPosStart = mousePos;
        m_dragPosStop = mousePos;

        updateDragRect();

        const SDL_Rect mapRect = context.layout.mapRect();
        m_isDragging = SDL_PointInRect(&mousePos, &mapRect);

        updateDragSelectedMapCells(context);

        context.sfx.play("tick-on-1");
    }

    void StateEditor::stopDragging(const Context & context, const SDL_Point & mousePos)
    {
        const MapPos_t newMapPos = context.map.screenPosToMapPos(context, mousePos);

        if ((newMapPos.x >= 0) && (newMapPos.y >= 0))
        {
            m_editPos = newMapPos;
            placeEditCursor(context);
            context.sfx.play("tick-off-1");
        }

        m_isDragging = false;

        const SDL_Rect mapRect = context.layout.mapRect();
        if (SDL_PointInRect(&mousePos, &mapRect))
        {
            updateDragRect();
        }
        else
        {
            m_dragRect = { 0, 0, 0, 0 };
        }
    }

    void StateEditor::updateDragging(const Context & context, const SDL_Point & mousePos)
    {
        m_dragPosStop = mousePos;
        updateDragRect();
        updateDragSelectedMapCells(context);
    }

    void StateEditor::setupKeyText(const Context & context)
    {
        if (m_keyTexturePtr != nullptr)
        {
            return;
        }

        const std::string keyText(
            "Esc-Quit\nCNTRL-s-Save\nSpace-Bare Floor\nPeriod-Erase\n"
            "1-Dirt Floor\n2-Stone Floor\n3-Wood Floor\na-Bag\n"
            "r-Rock\nl-Lava\nw-Water\ng-Slime\nc-Chest\nk-Coffin\n"
            "S-Stairs Up\ns-Stair Down\nD-Door Locked\nd-Door Unlocked\n"
            "0-Snake\n1-SnakeBag\n2-Spider\n3-Spiderweb\n4-Goblin\n5-GoblinBarrel\n"
            "6-Bat\n7-BatMask\n8-Skeleton\n9-SkeletonGrave\n:-Demon\n;-DemonDoor\n"
            "[-Dragon\n]-DragonInferno");

        util::destroyTexture(m_keyTexturePtr);

        m_keyTexturePtr = context.font.makeTexture(context, FontSize::Small, keyText);

        const SDL_Point size = util::size(m_keyTexturePtr);
        m_keyScreenPos.x = ((context.layout.topRect().w / 2) - (size.x / 2));
        m_keyScreenPos.y = (context.layout.topRect().h + 100);
    }

    void StateEditor::resetMap(const Context & context)
    {
        Map startingMap(MapName::Level_1_Cell, context, m_floor, m_mapChars, MapTransitions_t{});
        context.map = startingMap;
        context.layout.setupNewMap(context.map.size());
        context.map_display.load(context);
    }

    void StateEditor::placeEditCursor(const Context & context)
    {
        const SDL_Point pos = context.map.mapPosToScreenPos(context, m_editPos);
        m_editRect.x = pos.x;
        m_editRect.y = pos.y;
    }

    void StateEditor::setMapChar(const MapPos_t & pos, const char ch)
    {
        m_mapChars.at(static_cast<std::size_t>(pos.y)).at(static_cast<std::size_t>(pos.x)) = ch;
    }

    void StateEditor::fadeText(const Context & context, const std::string & text)
    {
        util::destroyTexture(m_fadeTexturePtr);

        if (text.empty())
        {
            return;
        }

        m_fadeTexturePtr = context.font.makeTexture(context, FontSize::Large, text);

        const SDL_Point size = util::size(m_fadeTexturePtr);
        m_fadeScreenPos.x = ((context.layout.topRect().w / 2) - (size.x / 2));
        m_fadeScreenPos.y = (size.y / 2);

        m_fadeTextAlpha = 255;
        SDL_SetTextureAlphaMod(m_fadeTexturePtr, m_fadeTextAlpha);
    }

    void StateEditor::editMap(
        const Context & context, const bool isShift, const char upper, const char lower)
    {
        const char mapCharToSet = ((isShift) ? upper : lower);

        if (m_dragSelectedEntrys.empty())
        {
            setMapChar(m_editPos, mapCharToSet);
        }
        else
        {
            for (const MapEntry_t & entry : m_dragSelectedEntrys)
            {
                setMapChar(entry.pos, mapCharToSet);
            }
        }

        if (isShift)
        {
            fadeText(context, mapCharToName(upper));
        }
        else
        {
            fadeText(context, mapCharToName(lower));
        }

        resetMap(context);
        context.sfx.play("tick-on-1");
    }

    void StateEditor::save() const
    {
        std::string filename{ "map.txt" };
        std::size_t filenum = 1;
        std::filesystem::path path{ filename };
        while (std::filesystem::exists(path))
        {
            filename = "map";
            filename += std::to_string(filenum++);
            filename += ".txt";

            path = { filename };
        };

        std::ofstream fStream(filename, std::ios::trunc);

        fStream << toString(m_floor) << std::endl;

        for (const std::string & rowStr : m_mapChars)
        {
            fStream << '\"' << rowStr << '\"' << std::endl;
        }
    }

    const std::string StateEditor::mapCharToName(const char ch) noexcept
    {
        // clang-format off
        switch (ch)
        {
            case ' ': { return "Bare Floor"; }
            case '.': { return "Erase"; }
            case 'D': { return "Door Locked"; }
            case 'd': { return "Door Unlocked"; }
            case 'S': { return "Stairs Up"; }
            case 's': { return "Stairs Down"; }
            case 'r': { return "Rock"; }
            case 'l': { return "Lava"; }
            case 'w': { return "Water"; }
            case 'g': { return "Slime"; }
            case 'b': { return "Barrel"; }
            case 'c': { return "Chest"; }
            case 'k': { return "Coffin"; }
            case '0': { return "Snake"; }
            case '1': { return "Snake Bag"; }
            case '2': { return "Spider"; }
            case '3': { return "Spiderweb"; }
            case '4': { return "Goblin"; }
            case '5': { return "Goblin Barrel"; }
            case '6': { return "Bat"; }
            case '7': { return "Bat Mask"; }
            case '8': { return "Skeleton"; }
            case '9': { return "Skeleton Grave"; }
            case ':': { return "Demon"; }
            case ';': { return "Demon Door"; }
            case '[': { return "Dragon"; }
            case ']': { return "Dragon Inferno"; }
            default:  { return ""; }
        }
        // clang-format on
    }

    void StateEditor::updateDragRect()
    {
        m_dragRect.x = util::min(m_dragPosStart.x, m_dragPosStop.x);
        m_dragRect.y = util::min(m_dragPosStart.y, m_dragPosStop.y);
        m_dragRect.w = util::abs(m_dragPosStart.x - m_dragPosStop.x);
        m_dragRect.h = util::abs(m_dragPosStart.y - m_dragPosStop.y);
    }

    void StateEditor::updateDragSelectedMapCells(const Context & context)
    {
        m_dragSelectedEntrys.clear();

        const SDL_Rect mapRect = context.layout.mapRect();
        const SDL_Point mapSize = context.layout.cellCount();

        SDL_Point screenPos = util::position(mapRect);
        for (int y(0); y < mapSize.y; ++y)
        {
            for (int x(0); x < mapSize.x; ++x)
            {
                const SDL_Rect screenRect = util::makeRect(screenPos, context.layout.cellSize());

                if (SDL_HasIntersection(&screenRect, &m_dragRect))
                {
                    const MapEntry_t entry{ { x, y }, screenRect };
                    m_dragSelectedEntrys.push_back(entry);
                }

                screenPos.x += context.layout.cellSize().x;
            }

            screenPos.x = mapRect.x;
            screenPos.y += context.layout.cellSize().y;
        }
    }

} // namespace castlecrawl
