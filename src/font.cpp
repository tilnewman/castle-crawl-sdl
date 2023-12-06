// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// font.cpp
//
#include "font.hpp"

#include "context.hpp"
#include "game-config.hpp"
#include "sdl-manager.hpp"
#include "sdl-util.hpp"
#include "sfml-util.hpp"

#include <SFML/Graphics/Text.hpp>

#include <string>

namespace castlecrawl
{

    FontManager::FontManager()
        : m_font()
    {}

    void FontManager::setup(const GameConfig & config)
    {
        m_font.loadFromFile(
            (config.media_path / "font" / "gentium-plus" / "gentium-plus.ttf").string());

        setupFontExtents(config);
    }

    const sf::Text FontManager::makeText(
        const FontSize size, const std::string & str, const SDL_Color & color) const
    {
        sf::Text text;

        sf::Color sfmlColor;
        sfmlColor.r = color.r;
        sfmlColor.g = color.g;
        sfmlColor.b = color.b;
        sfmlColor.a = color.a;
        text.setFillColor(sfmlColor);

        text.setFont(m_font);
        text.setCharacterSize(extent(size).char_size);
        text.setString(str);

        util::setOriginToPosition(text);

        return text;
    }

    SDL_Texture * FontManager::makeTexture(
        const Context & context,
        const FontSize size,
        const std::string & str,
        const SDL_Color & color) const
    {
        return context.sdl.makeTextTexture(context.font.makeText(size, str, color));
    }

    void FontManager::setupFontExtents(const GameConfig & config)
    {
        // these values are based on default MacBook Pros resolution and are close enough to others
        const float standardRes = std::sqrt(3840.f * 2400.0f);

        const float currentRes =
            std::sqrt(static_cast<float>(config.resolution.x * config.resolution.y));

        const float ratioRes = (currentRes / standardRes);

        sf::Text text;
        const std::string widthStr{ "M" };
        const std::string heightStr{ "|g" };

        m_fontExtentHuge.char_size = static_cast<unsigned>(130.0f * ratioRes);
        text = makeText(FontSize::Huge, widthStr);
        m_fontExtentHuge.letter_size.x = static_cast<int>(text.getGlobalBounds().width);
        text = makeText(FontSize::Huge, heightStr);
        m_fontExtentHuge.letter_size.y = static_cast<int>(text.getGlobalBounds().height);

        m_fontExtentLarge.char_size = static_cast<unsigned>(90.0f * ratioRes);
        text = makeText(FontSize::Large, widthStr);
        m_fontExtentLarge.letter_size.x = static_cast<int>(text.getGlobalBounds().width);
        text = makeText(FontSize::Large, heightStr);
        m_fontExtentLarge.letter_size.y = static_cast<int>(text.getGlobalBounds().height);

        m_fontExtentMedium.char_size = static_cast<unsigned>(60.0f * ratioRes);
        text = makeText(FontSize::Medium, widthStr);
        m_fontExtentMedium.letter_size.x = static_cast<int>(text.getGlobalBounds().width);
        text = makeText(FontSize::Medium, heightStr);
        m_fontExtentMedium.letter_size.y = static_cast<int>(text.getGlobalBounds().height);

        m_fontExtentSmall.char_size = static_cast<unsigned>(40.0f * ratioRes);
        text = makeText(FontSize::Small, widthStr);
        m_fontExtentSmall.letter_size.x = static_cast<int>(text.getGlobalBounds().width);
        text = makeText(FontSize::Small, heightStr);
        m_fontExtentSmall.letter_size.y = static_cast<int>(text.getGlobalBounds().height);
    }

} // namespace castlecrawl
