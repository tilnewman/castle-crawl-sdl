#ifndef CASTLECRAWL2_FONT_HPP_INCLUDED
#define CASTLECRAWL2_FONT_HPP_INCLUDED
//
// font.hpp
//
#include <SDL.h>

#include <SFML/Graphics/Font.hpp>

namespace castlecrawl
{
    struct Context;
    struct GameConfig;

    enum class FontSize
    {
        Huge,
        Large,
        Medium,
        Small
    };

    struct FontExtent
    {
        unsigned int char_size = 0;
        SDL_Point letter_size;
    };

    class FontManager
    {
      public:
        FontManager();

        void setup(const GameConfig & config);
        const sf::Font & font() const { return m_font; }

        inline const FontExtent extent(const FontSize size) const noexcept
        {
            // clang-format off
            switch (size)
            {
                case FontSize::Huge:    { return m_fontExtentHuge; }
                case FontSize::Large:   { return m_fontExtentLarge; }
                case FontSize::Medium:  { return m_fontExtentMedium; }
                case FontSize::Small:  
                default:                { return m_fontExtentSmall; }
            }
            // clang-format on
        }

        const sf::Text makeText(
            const FontSize size,
            const std::string & str,
            const SDL_Color & color = { 255, 255, 255, 255 }) const;

        SDL_Texture * makeTexture(
            const Context & context,
            const FontSize size,
            const std::string & str,
            const SDL_Color & color = { 255, 255, 255, 255 }) const;

      private:
        void setupFontExtents(const GameConfig & config);

      private:
        sf::Font m_font;

        FontExtent m_fontExtentHuge;
        FontExtent m_fontExtentLarge;
        FontExtent m_fontExtentMedium;
        FontExtent m_fontExtentSmall;
    };

} // namespace castlecrawl

#endif // CASTLECRAWL2_FONT_HPP_INCLUDED
