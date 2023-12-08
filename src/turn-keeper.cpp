// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// turn-keeper.cpp
//
#include "context.hpp"
#include "enemy.hpp"
#include "turn-keeper.hpp"

namespace castlecrawl
{

    TurnKeeper::TurnKeeper()
        : m_isPlayerTurn(true)
    {}

    void TurnKeeper::passTurn(const Context & context)
    {
        if (isPlayerTurn())
        {
            if (context.enemy.isAnyNonSummoner())
            {
                context.enemy.takeTurns(context);
            }
        }
    }

} // namespace castlecrawl
