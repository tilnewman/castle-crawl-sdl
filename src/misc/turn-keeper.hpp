#ifndef TURNKEEPER_HPP_INCLUDED
#define TURNKEEPER_HPP_INCLUDED
//
// turn-keeper.hpp
//

namespace castlecrawl
{
    struct Context;

    class TurnKeeper
    {
      public:
        TurnKeeper();

        bool isPlayerTurn() const { return m_isPlayerTurn; }
        void passTurn(const Context & context);

      private:
        bool m_isPlayerTurn;
    };

} // namespace castlecrawl

#endif // TURNKEEPER_HPP_INCLUDED
