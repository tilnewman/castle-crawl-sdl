// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// quick-map.cpp
//
#include "quick-map.hpp"

#include "check-macros.hpp"

#include <iostream>

namespace castlecrawl
{

    QuickMap::QuickMap(const GameConfig & config, const MapChars_t & mapChars)
        : m_mapChars(mapChars)
    {
        convertQuickMapToRealMap(config);
    }

    char QuickMap::getChar(const int x, const int y) const
    {
        if (empty() || !isPosValid({ x, y }))
        {
            return '.';
        }
        else
        {
            return m_mapChars.at(static_cast<std::size_t>(y)).at(static_cast<std::size_t>(x));
        }
    }

    void QuickMap::dumpToConsole() const
    {
        for (const std::string & row : m_mapChars)
        {
            std::cout << row << std::endl;
        }
    }

    void QuickMap::convertQuickMapToRealMap(const GameConfig & config)
    {
        convertQuickMapToRealMap_VerifySize(config);
        convertQuickMapToRealMap_Walls();
        convertQuickMapToRealMap_WallCorners();
    }

    void QuickMap::convertQuickMapToRealMap_VerifySize(const GameConfig &) const
    {
        M_CHECK(!empty(), "QuickMap constructor given an empty map!");

        const std::size_t rowLength = m_mapChars.front().size();
        for (const std::string & row : m_mapChars)
        {
            M_CHECK(
                (rowLength == row.size()),
                "QuickMap constrcutor given a map with rows of different lengths!");
        }
    }

    void QuickMap::convertQuickMapToRealMap_Walls()
    {
        if (empty())
        {
            return;
        }

        auto isNotObs = [](const char ch) {
            return ((ch != '.') && (ch != '|') && (ch != '-') && (ch != 'r'));
        };

        const MapPos_t mapSize = size();

        for (int y(0); y < mapSize.y; ++y)
        {
            for (int x(0); x < mapSize.x; ++x)
            {
                const char ch{ getChar(x, y) };

                if (ch != '.')
                {
                    continue;
                }

                // check all four sides
                const char up{ getChar(x, y - 1) };
                const char down{ getChar(x, y + 1) };
                const char left{ getChar(x - 1, y) };
                const char right{ getChar(x + 1, y) };

                if (isNotObs(left) || isNotObs(right))
                {
                    setChar(x, y, '|');
                    continue;
                }
                else if (isNotObs(up) || isNotObs(down))
                {
                    setChar(x, y, '-');
                    continue;
                }

                // check all four corners
                const char tl{ getChar(x - 1, y - 1) };
                const char tr{ getChar(x + 1, y - 1) };
                const char bl{ getChar(x - 1, y + 1) };
                const char br{ getChar(x + 1, y + 1) };

                if (isNotObs(tl) || isNotObs(tr) || isNotObs(bl) || isNotObs(br))
                {
                    setChar(x, y, '|'); // '-' works here too
                    continue;
                }
            }
        }
    }

    void QuickMap::convertQuickMapToRealMap_WallCorners()
    {
        if (empty())
        {
            return;
        }

        const MapPos_t mapSize(size());

        for (int y(0); y < mapSize.y; ++y)
        {
            for (int x(0); x < mapSize.x; ++x)
            {
                const char ch{ getChar(x, y) };

                if ((ch != '|') && (ch != '-'))
                {
                    continue;
                }

                const char up{ getChar(x, y - 1) };
                const char down{ getChar(x, y + 1) };
                const char left{ getChar(x - 1, y) };
                const char right{ getChar(x + 1, y) };

                if (ch == '|')
                {
                    if ((left == '-') || (right == '-'))
                    {
                        setChar(x, y, 'B');
                    }

                    if ((down == '|') && ((up == ' ') || (up == '.')))
                    {
                        setChar(x, y, 'B');
                    }

                    if ((up == '|') && ((down == ' ') || (down == '.')))
                    {
                        setChar(x, y, 'B');
                    }
                }
                else //'-'
                {
                    if ((up == '|') || (down == '|'))
                    {
                        setChar(x, y, 'B');
                    }

                    if ((left == '-') && ((right == ' ') || (right == '.')))
                    {
                        setChar(x, y, 'B');
                    }

                    if ((right == '-') && ((left == ' ') || (left == '.')))
                    {
                        setChar(x, y, 'B');
                    }
                }

                if (getChar(x, y) != 'B')
                {
                    continue;
                }

                if ((down == '|') || (down == '-'))
                {
                    if ((right == '|') || (right == '-'))
                    {
                        setChar(x, y, 'C');
                    }
                    else
                    {
                        setChar(x, y, 'T');
                    }
                }
            }
        }
    }

} // namespace castlecrawl
