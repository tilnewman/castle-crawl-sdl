// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// clock.cpp
//
#include "clock.hpp"

#include "check-macros.hpp"
#include "context.hpp"
#include "game-config.hpp"
#include "sdl-manager.hpp"

namespace castlecrawl
{

    Clock::Clock()
        : m_startTime(std::chrono::high_resolution_clock::now())
    {}

    float Clock::restart()
    {
        using namespace std::chrono;
        const auto now = high_resolution_clock::now();

        const auto elapsedNS = static_cast<double>(round<nanoseconds>(now - m_startTime).count());

        m_startTime = now;

        const double nanoSecondsPerSecond = 1000000000.0;
        return static_cast<float>(elapsedNS / nanoSecondsPerSecond);
    }

} // namespace castlecrawl
