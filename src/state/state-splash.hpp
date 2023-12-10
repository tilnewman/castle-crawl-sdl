#ifndef STATESPLASH_HPP_INCLUDED
#define STATESPLASH_HPP_INCLUDED
//
// state-splash.hpp
//
#include "display/fader.hpp"
#include "state/state.hpp"

namespace castlecrawl
{
    struct Context;

    class StateSplash : public StateBase
    {
      public:
        StateSplash();
        virtual ~StateSplash() final {}

        virtual State which() const final { return State::Splash; }
        virtual void onEnter(const Context & context) final;
        virtual void onExit(const Context & context) final;
        virtual void update(const Context & context, const float frameTimeSec) final;
        virtual void draw(const Context & context) const final;
        virtual void handleEvent(const Context & context, const SDL_Event & event) final;

      private:
        SDL_Texture * m_castleTexturePtr;
        SDL_Rect m_castleSrcRect;
        SDL_Rect m_castleDestRect;
        SDL_Texture * m_lightningTexturePtr;
        SDL_Rect m_lightningSrcRect;
        SDL_Rect m_lightningDestRect;
        bool m_willShowLightning;
        float m_timerSec;
        Fader m_fader;
    };

} // namespace castlecrawl

#endif // STATESPLASH_HPP_INCLUDED
