#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED
//
// player.hpp
//
#include "player/stat.hpp"

namespace castlecrawl
{

    class Player
    {
      public:
        Player();

        Stat & strength() { return m_strength; }
        Stat & dexterity() { return m_dexterity; }
        Stat & accuracy() { return m_accuracy; }
        Stat & luck() { return m_luck; }
        Stat & arcane() { return m_arcane; }

        int health() const { return m_health; }
        int level() const { return m_level; }
        int mana() const { return m_mana; }
        int gold() const { return m_gold; }

        void healthAdj(const int adj) { m_health += adj; }
        void levelAdj(const int adj) { m_level += adj; }
        void manaAdj(const int adj) { m_mana += adj; }
        void goldAdj(const int adj) { m_gold += adj; }

      private:
        constexpr static int statMin = 1;
        constexpr static int statMax = 100;
        constexpr static int statStart = 10;
        constexpr static int healthStart = 20;
        constexpr static int manaStart = 10;

        Stat m_strength;
        Stat m_dexterity;
        Stat m_arcane;
        Stat m_accuracy;
        Stat m_luck;

        int m_health;
        int m_level;
        int m_mana;
        int m_gold;
    };

} // namespace castlecrawl

#endif // PLAYER_HPP_INCLUDED
