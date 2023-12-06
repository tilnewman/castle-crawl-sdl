// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// state-manager.cpp
//
#include "state-manager.hpp"

#include "check-macros.hpp"
#include "state-editor.hpp"
#include "state-play.hpp"
#include "state-splash.hpp"

#include <exception>

namespace castlecrawl
{

    StateManager::StateManager()
        : m_stateUPtr(factory(State::Init))
    {}

    void StateManager::change(const Context & context, const State newState)
    {
        const State currentState = m_stateUPtr->which();

        M_CHECK(
            (newState != currentState), "Tried to change to the same State! (" << newState << ")");

        m_stateUPtr->onExit(context);

        m_stateUPtr.release();
        m_stateUPtr = factory(newState);

        m_stateUPtr->onEnter(context);
    }

    std::unique_ptr<IState> StateManager::factory(const State state) const
    {
        switch (state)
        {
            // clang-format off
            case State::Init:   { return std::make_unique<StateInit>(); } 
            case State::Splash: { return std::make_unique<StateSplash>(); }
            case State::Play:   { return std::make_unique<StatePlay>(); }
            case State::Quit:   { return std::make_unique<StateQuit>(); }
            case State::Editor: { return std::make_unique<StateEditor>(); }
            default:
            {
                throw std::runtime_error("Error: state-manager::factory() called with unknown state!  Did you add a state?");
            }
                // clang-format on
        }
    }

} // namespace castlecrawl
