// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// tile-images.cpp
//
#include "display/tile-images.hpp"

#include "misc/check-macros.hpp"
#include "misc/context.hpp"
#include "misc/game-config.hpp"
#include "display/sdl-manager.hpp"

namespace castlecrawl
{

    TileImages::TileImages()
        : m_texturePtr(nullptr)
    {}

    void TileImages::setup(const Context & context)
    {
        m_texturePtr =
            context.sdl.loadTexture((context.config.media_path / "image" / "tile.png").string());
    }

    void TileImages::teardown()
    {
        if (m_texturePtr != nullptr)
        {
            SDL_DestroyTexture(m_texturePtr);
        }
    }

} // namespace castlecrawl
