#ifndef STAT_HPP_INCLUDED
#define STAT_HPP_INCLUDED
//
// stat.hpp
//
#include <algorithm>

namespace castlecrawl
{

    // Tracks a player's stat (i.e. strength) which is the m_normal value that
    // is always clamped between a min and max, but can be temporarily changed
    // by magical items etc so this also tracks a m_current value that can
    // fluctuate higher or lower.
    class Stat
    {
      public:
        Stat(const int value, const int theMin, const int theMax)
            : m_current(value)
            , m_normal(value)
            , m_min(theMin)
            , m_max(theMax)
        {}

        int current() const { return m_current; }
        int normal() const { return m_normal; }

        int min() const { return m_min; }
        int max() const { return m_max; }

        void adjCurrent(const int adjustment) { current(m_current + adjustment); }
        void adjNormal(const int adjustment) { normal(m_normal + adjustment); }

        void adjCurrentNormalClamped(const int adjustment)
        {
            adjCurrent(adjustment);
            m_current = std::clamp(m_current, m_min, m_normal);
        }

        void current(const int newValue)
        {
            m_current = newValue;
            m_current = std::clamp(m_current, m_min, m_max);
        }

        void normal(const int newValue)
        {
            m_normal = newValue;
            m_normal = std::clamp(m_normal, m_min, m_max);
        }

        void reset() { m_current = m_normal; }

        float ratio() const
        {
            return (static_cast<float>(m_current) / static_cast<float>(m_normal));
        }

      private:
        int m_current;
        int m_normal;
        int m_min;
        int m_max;
    };

} // namespace castlecrawl

#endif // STAT_HPP_INCLUDED
