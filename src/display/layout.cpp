// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// layout.hpp
//
#include "display/layout.hpp"

#include "misc/check-macros.hpp"
#include "misc/sdl-util.hpp"

#include <algorithm>
#include <iostream>

namespace castlecrawl
{

    Layout::Layout()
        : m_screenRect{ 0, 0, 0, 0 }
        , m_topRect{ 0, 0, 0, 0 }
        , m_botRect{ 0, 0, 0, 0 }
        , m_mapRect{ 0, 0, 0, 0 }
        , m_cellSize{ 0, 0 }
        , m_cellCount{ 0, 0 }
    {}

    void Layout::setup(const GameConfig & config)
    {
        m_screenRect.w = config.resolution.x;
        m_screenRect.h = config.resolution.y;

        m_topRect = m_screenRect;

        m_topRect.h =
            static_cast<int>(static_cast<float>(m_screenRect.h) * config.top_panel_height_ratio);

        m_botRect = m_screenRect;
        m_botRect.y = m_topRect.h;
        m_botRect.h -= m_botRect.y;

        m_cellSize.x = std::min(
            m_botRect.w / config.map_cell_size_max.x, m_botRect.h / config.map_cell_size_max.y);

        m_cellSize.y = m_cellSize.x;
    }

    void Layout::setupNewMap(const SDL_Point & cellCountParam)
    {
        m_cellCount = cellCountParam;

        const SDL_Point boardSize{ m_cellSize * cellCountParam };
        const SDL_Point boardPos{ util::center(m_botRect) - (boardSize / 2) };
        m_mapRect = util::makeRect(boardPos, boardSize);
    }

} // namespace castlecrawl
