// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// state-play.cpp
//
#include "state-play.hpp"

#include "animation-player.hpp"
#include "check-macros.hpp"
#include "context.hpp"
#include "enemy.hpp"
#include "font.hpp"
#include "framerate-text.hpp"
#include "layout.hpp"
#include "map-display.hpp"
#include "map-objects.hpp"
#include "map.hpp"
#include "maps.hpp"
#include "mouseover.hpp"
#include "music-player.hpp"
#include "player-display.hpp"
#include "sdl-manager.hpp"
#include "sound-player.hpp"
#include "state-manager.hpp"
#include "top-panel.hpp"
#include "turn-keeper.hpp"

namespace castlecrawl
{

    StatePlay::StatePlay()
        : m_mouseover()
        , m_fader()
        , m_mapFadeDurationSec(1.0f)
        , m_transition()
    {}

    void StatePlay::onEnter(const Context & context)
    {
        m_transition = MapTransition{ { -1, -1 }, MapName::Level_1_Cell, { 3, 2 } };

        m_fader.setup(
            true, context.config.background_color, m_mapFadeDurationSec, context.layout.mapRect());

        m_mouseover.setup(context);
    }

    void StatePlay::onExit(const Context &) { m_mouseover.teardown(); }

    void StatePlay::update(const Context & context, const float frameTimeSec)
    {
        const bool didMapJustFinishFading = m_fader.update(frameTimeSec);
        if (didMapJustFinishFading)
        {
            context.maps.change(context, m_transition.to_name, m_transition.to_pos);

            if (m_fader.isFadingIn())
            {
                m_fader.reset();
            }
            else
            {
                m_fader.setup(
                    true,
                    context.config.background_color,
                    m_mapFadeDurationSec,
                    context.layout.mapRect());
            }
        }

        if (!m_fader.isFading())
        {
            context.enemy.update(context, frameTimeSec);
            m_mouseover.update(context, frameTimeSec);
        }

        context.player.update(frameTimeSec);
        context.framerate.update(context, frameTimeSec);
        context.anim.update(frameTimeSec);
    }

    void StatePlay::draw(const Context & context) const
    {
        context.map_display.draw(context);
        context.map_object.draw(context);
        context.enemy.draw(context);

        if (!m_fader.isFading())
        {
            context.player.draw(context);
            m_mouseover.draw(context);
        }

        m_fader.draw(context);

        context.framerate.draw(context);
        context.top_panel.draw(context);
        context.anim.draw(context.sdl);
    }

    void StatePlay::handleEvent(const Context & context, const SDL_Event & event)
    {
        if (m_fader.isFading())
        {
            return;
        }

        if (event.type == SDL_MOUSEMOTION)
        {
            m_mouseover.handleMouseMovedEvent({ event.motion.x, event.motion.y });
            return;
        }

        // all other handlers are key released events
        if (SDL_KEYUP != event.type)
        {
            return;
        }

        // all other handlers are only valid if it's the player's turn
        if (!context.turn.isPlayerTurn())
        {
            return;
        }

        if ((event.key.keysym.sym == SDLK_UP) || (event.key.keysym.sym == SDLK_DOWN) ||
            (event.key.keysym.sym == SDLK_LEFT) || (event.key.keysym.sym == SDLK_RIGHT))
        {
            handlePlayerMove(context, event.key.keysym.sym);
            return;
        }
    }

    void StatePlay::handlePlayerMove(const Context & context, const SDL_Keycode key)
    {
        const MapPos_t mapPosBefore = context.player.position();
        const MapPos_t mapPosAttempted = moveIfDir(mapPosBefore, key);
        const bool isEnemyInTheWay = context.enemy.isAnyAtMapPos(mapPosAttempted);
        const bool isObjectInTheWay = context.map_object.isAnyAtMapPos(mapPosAttempted);
        const char mapCharAttempted = context.map.cell(mapPosAttempted).object_char;

        const MapPos_t mapPosAfter = [&]() {
            if (isEnemyInTheWay || isObjectInTheWay)
            {
                return mapPosBefore;
            }
            else if (
                (mapCharAttempted == ' ') || (mapCharAttempted == 'd') ||
                (mapCharAttempted == '.') || (mapCharAttempted == 's') || (mapCharAttempted == 'S'))
            {
                return mapPosAttempted;
            }
            else
            {
                return mapPosBefore;
            }
        }();

        const bool didMove = (mapPosBefore != mapPosAfter);

        playMoveSfx(context, didMove, mapCharAttempted);

        if (didMove)
        {
            context.player.position(context, mapPosAfter);

            if (!handleMapTransition(context, mapPosAfter))
            {
                context.turn.passTurn(context);
            }

            playMoveMusic(context);
        }
    }

    void StatePlay::playMoveSfx(
        const Context & context, const bool didMove, const char mapCharAttempted) const
    {
        if (didMove)
        {
            if (mapCharAttempted == 'd')
            {
                context.sfx.play("door-open");
            }
            else if ((mapCharAttempted == 's') || (mapCharAttempted == 'S'))
            {
                context.sfx.play("stairs");
            }
            else
            {
                context.sfx.play("tick-off-1");
            }
        }
        else
        {
            if (mapCharAttempted == 'D')
            {
                context.sfx.play("locked");
            }
            else if (mapCharAttempted == 'w')
            {
                context.sfx.play("splash");
            }
            else if (mapCharAttempted == 'l')
            {
                context.sfx.play("burn");
            }
            else
            {
                context.sfx.play("tap-wood-low");
            }
        }
    }

    void StatePlay::playMoveMusic(const Context & context) const
    {
        const auto surroundingCells = context.map.surroundingCellsAll(context.player.position());

        const auto foundLavaIter = std::find_if(
            std::begin(surroundingCells), std::end(surroundingCells), [](const MapCell & cell) {
                return (cell.object_char == 'l');
            });

        if (foundLavaIter == std::end(surroundingCells))
        {
            context.music.stop("lava.ogg");
        }
        else
        {
            context.music.start("lava.ogg");
        }

        const auto foundWaterIter = std::find_if(
            std::begin(surroundingCells), std::end(surroundingCells), [](const MapCell & cell) {
                return (cell.object_char == 'w');
            });

        if (foundWaterIter == std::end(surroundingCells))
        {
            context.music.stop("water-bubbles.ogg");
        }
        else
        {
            context.music.start("water-bubbles.ogg");
        }
    }

    bool StatePlay::handleMapTransition(const Context & context, const MapPos_t & mapPosAfter)
    {
        for (const MapTransition & transition : context.map.transitions())
        {
            if (transition.from_pos == mapPosAfter)
            {
                m_transition = transition;

                m_fader.setup(
                    false,
                    context.config.background_color,
                    m_mapFadeDurationSec,
                    context.layout.mapRect());

                m_mouseover.reset();
                return true;
            }
        }

        return false;
    }

    const MapPos_t StatePlay::moveIfDir(const MapPos_t & oldPos, const SDL_Keycode dir)
    {
        MapPos_t newPos{ oldPos };

        // clang-format off
        if      (dir == SDLK_UP)    { --newPos.y; }
        else if (dir == SDLK_DOWN)  { ++newPos.y; }
        else if (dir == SDLK_LEFT)  { --newPos.x; }
        else if (dir == SDLK_RIGHT) { ++newPos.x; }
        // clang-format on

        return newPos;
    }

} // namespace castlecrawl
