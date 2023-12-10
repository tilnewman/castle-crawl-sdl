#ifndef STATEPLAY_HPP_INCLUDED
#define STATEPLAY_HPP_INCLUDED
//
// state-play.hpp
//
#include "display/fader.hpp"
#include "display/mouseover.hpp"
#include "state/state.hpp"

#include <SDL.h>

namespace castlecrawl
{
    struct Context;

    class StatePlay : public StateBase
    {
      public:
        StatePlay();
        virtual ~StatePlay() final {}

        virtual void onEnter(const Context & context) final;
        virtual void onExit(const Context & context) final;
        virtual State which() const final { return State::Play; }
        virtual void update(const Context & context, const float frameTimeSec) final;
        virtual void draw(const Context & context) const final;
        virtual void handleEvent(const Context & context, const SDL_Event & event) final;

      private:
        void handlePlayerMove(const Context & context, const SDL_Keycode key);

        void playMoveSfx(
            const Context & context, const bool didMove, const char mapCharAttempted) const;

        void playMoveMusic(const Context & context) const;
        bool handleMapTransition(const Context & context, const MapPos_t & mapPosAfter);
        const MapPos_t moveIfDir(const MapPos_t & oldPos, const SDL_Keycode dir);

      private:
        Mouseover m_mouseover;
        Fader m_fader;
        float m_mapFadeDurationSec;
        MapTransition m_transition;
    };

} // namespace castlecrawl

#endif // STATEPLAY_HPP_INCLUDED
