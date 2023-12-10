// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// player.cpp
//
#include "player/player.hpp"

namespace castlecrawl
{

    Player::Player()
        : m_strength(statStart, statMin, statMax)
        , m_dexterity(statStart, statMin, statMax)
        , m_arcane(statStart, statMin, statMax)
        , m_accuracy(statStart, statMin, statMax)
        , m_luck(statStart, statMin, statMax)
        , m_health(healthStart)
        , m_level(1)
        , m_mana(manaStart)
        , m_gold(0)
    {}

} // namespace castlecrawl
