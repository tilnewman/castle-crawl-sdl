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
        m_titleTexturePtr =
            context.sdl.loadTexture((context.config.media_path / "image" / "title.png").string());

        SDL_QueryTexture(m_titleTexturePtr, nullptr, nullptr, &m_titleSrcRect.w, &m_titleSrcRect.h);

        m_titleDestRect = context.layout.topRect();

        util::fit(
            m_titleDestRect,
            static_cast<int>(static_cast<float>(context.layout.topRect().w) * 0.4f),
            static_cast<int>(context.layout.topRect().h));

        m_titleDestRect.y = 0;

        m_titleDestRect.x =
            static_cast<int>((context.layout.screenRect().w * 0.5f) - (m_titleDestRect.w * 0.5f));
    }

    void TopPanel::teardown()
    {
        if (m_titleTexturePtr != nullptr)
        {
            SDL_DestroyTexture(m_titleTexturePtr);
        }
    }

    void TopPanel::draw(const Context & context) const
    {
        context.sdl.blit(m_titleTexturePtr, m_titleSrcRect, m_titleDestRect);
    }

} // namespace castlecrawl
