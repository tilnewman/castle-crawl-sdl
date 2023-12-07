// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// turn-keeper.cpp
//
#include "turn-keeper.hpp"

namespace castlecrawl
{

    TurnKeeper::TurnKeeper()
        : m_isPlayerTurn(true)
        , m_elapsedSec(0.0f)
    {}

    void TurnKeeper::passTurn(const Context & context)
    {
        // TODO check for enemies
    }

} // namespace castlecrawl
