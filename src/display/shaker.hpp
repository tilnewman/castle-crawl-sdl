#ifndef SHAKER_HPP_INCLUDED
#define SHAKER_HPP_INCLUDED
//
// shaker.hpp
//

namespace castlecrawl
{

    class Shaker
    {
      public:
        Shaker();

        void setup(const int range, float speed);
        int update(const float frameTimeSec);
        int adjustment() const { return m_current; }

      private:
        int m_range;
        float m_speed;
        float m_elapsedTimeSec;
        int m_current;
    };

} // namespace castlecrawl

#endif // STATE_HPP_INCLUDED
