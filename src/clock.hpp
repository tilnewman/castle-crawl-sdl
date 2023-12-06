#ifndef CLOCK_HPP_INCLUDED
#define CLOCK_HPP_INCLUDED
//
// clock.hpp
//
#include <chrono>

namespace castlecrawl
{
    class Clock
    {
      public:
        Clock();

        // returns the elapsed time in seconds
        float restart();

      private:
        std::chrono::high_resolution_clock::time_point m_startTime;
    };

} // namespace castlecrawl

#endif // CLOCK_HPP_INCLUDED
