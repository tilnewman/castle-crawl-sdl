#ifndef STATE_HPP_INCLUDED
#define STATE_HPP_INCLUDED
//
// state.hpp
//
#include "map/map-types.hpp"

#include <ostream>
#include <string_view>

#include <SDL.h>

namespace castlecrawl
{
    struct Context;

    enum class State
    {
        Init,
        Splash,
        Play,
        Quit,
        Editor
    };

    inline constexpr std::string_view toString(const State state) noexcept
    {
        switch (state)
        {
            // clang-format off
            case State::Init:   { return "Init"; }
            case State::Splash: { return "Splash"; }
            case State::Play:   { return "Play"; }
            case State::Quit:   { return "Quit"; }
            case State::Editor: { return "Editor"; }
            default:            { return "(unknown_State)"; }
                // clang-format on
        }
    }

    inline std::ostream & operator<<(std::ostream & os, const State state)
    {
        os << toString(state);
        return os;
    }

    struct IState
    {
        virtual ~IState() {}

        virtual State which() const = 0;
        virtual void onEnter(const Context & context) = 0;
        virtual void onExit(const Context & context) = 0;
        virtual void update(const Context & context, const float frameTimeSec) = 0;
        virtual void draw(const Context & context) const = 0;
        virtual void handleEvent(const Context & context, const SDL_Event & event) = 0;
    };

    class StateBase : public IState
    {
      public:
        StateBase() {}
        virtual ~StateBase() override {}

        virtual void onEnter(const Context &) override {}
        virtual void onExit(const Context &) override {}
        virtual void update(const Context &, const float) override {}
        virtual void draw(const Context &) const override {}
        virtual void handleEvent(const Context &, const SDL_Event &) override {}
    };

    class StateInit : public StateBase
    {
      public:
        StateInit() {}
        virtual ~StateInit() final {}
        virtual State which() const final { return State::Init; }
    };

    class StateQuit : public StateBase
    {
      public:
        StateQuit() {}
        virtual ~StateQuit() final {}
        virtual State which() const final { return State::Quit; }
    };

} // namespace castlecrawl

#endif // STATE_HPP_INCLUDED
