// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// top-panel.cpp
//
#include "top-panel.hpp"

#include "context.hpp"
#include "layout.hpp"
#include "sdl-manager.hpp"
#include "sdl-util.hpp"

namespace castlecrawl
{

    TopPanel::TopPanel()
        : m_titleTexturePtr(nullptr)
        , m_titleDestRect{ 0, 0, 0, 0 }
        , m_titleSrcRect{ 0, 0, 0, 0 }
    {}

    void TopPanel::setup(const Context & context)
    {
        const SDL_Point titleImageSize{ static_cast<int>(
                                            static_cast<float>(context.layout.topRect().w) * 0.4f),
                                        context.layout.topRect().h };

        m_titleTexturePtr = context.sdl.loadAndSmoothResizeTexture(
            (context.config.media_path / "image" / "title.png").string(), titleImageSize);

        SDL_QueryTexture(m_titleTexturePtr, nullptr, nullptr, &m_titleSrcRect.w, &m_titleSrcRect.h);

        m_titleDestRect.y = 0;

        m_titleDestRect.x =
            static_cast<int>((context.layout.screenRect().w * 0.5f) - (m_titleDestRect.w * 0.5f));
    }

    void TopPanel::teardown() { util::destroyTexture(m_titleTexturePtr); }

    void TopPanel::draw(const Context & context) const
    {
        if (m_titleTexturePtr != nullptr)
        {
            context.sdl.blit(m_titleTexturePtr, m_titleSrcRect, m_titleDestRect);
        }
    }

} // namespace castlecrawl
