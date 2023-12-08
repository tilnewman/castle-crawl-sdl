// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// shaker.cpp
//
#include "shaker.hpp"

#include <cmath>

namespace castlecrawl
{

    Shaker::Shaker()
        : m_range(0)
        , m_speed(0.0f)
        , m_elapsedTimeSec(0.0f)
        , m_current(0)
    {}

    void Shaker::setup(const int range, float speed)
    {
        m_range = range; // try (context.layout.cellSize().x) * 0.1f)
        m_speed = speed; // try 75.0f
        m_elapsedTimeSec = 0.0f;
        m_current = 0;
    }

    int Shaker::update(const float frameTimeSec)
    {
        m_elapsedTimeSec += frameTimeSec;

        m_current = static_cast<int>(sin(m_elapsedTimeSec * m_speed) * static_cast<float>(m_range));

        return m_current;
    }

} // namespace castlecrawl
