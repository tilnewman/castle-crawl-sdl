#ifndef SDLMANAGER_HPP_INCLUDED
#define SDLMANAGER_HPP_INCLUDED
//
// sdl-manager.hpp
//
#include "map-types.hpp"

#include <SDL.h>

#include <string>
#include <vector>

namespace sf
{
    class Text;
}

namespace castlecrawl
{
    struct SDLManagerFlags
    {
        unsigned sdl_init = SDL_INIT_EVERYTHING;
        unsigned renderer = SDL_RENDERER_ACCELERATED;
        unsigned window = SDL_WINDOW_FULLSCREEN;
    };

    class SDLManager
    {
      public:
        SDLManager();

        void setup(
            const std::string & title,
            const int width,
            const int height,
            const bool willVSync,
            const SDLManagerFlags & flags = {});

        void teardown();

        void renderClear(const SDL_Color & color = { 0, 0, 0, 255 });
        void renderPresent();

        void blit(
            SDL_Texture * const texturePtr, const SDL_Rect & srcRect, const SDL_Rect & destRect);

        void blit(SDL_Texture * const texturePtr, int x, int y);
        void blit(SDL_Texture * const texturePtr, const BlitRectsVec_t & rects);
        void draw(const SDL_Color & color, const RectVec_t & rects);
        void draw(const SDL_Color & color, const SDL_Rect & rect);
        void drawOutline(const SDL_Color & color, const SDL_Rect & rect);

        SDL_Texture * loadTexture(const std::string & path) const;

        SDL_Texture *
            loadAndSmoothResizeTexture(const std::string & path, const SDL_Point & size) const;

        // SDL_Texture *
        //    loadAndMaskTexture(const std::string & path, const SDL_Color & maskColor) const;

        // const SDL_Color getPixel(SDL_Surface * const surfacePtr, const int x, const int y) const;

        // assumes the surface is RGBA8888
        void setPixel(
            SDL_Surface * surfacePtr, const int x, const int y, const unsigned color) const;

        SDL_Texture * makeTextTexture(const sf::Text & sfmlText);

        void dumpDisplayDrivers() const;
        void dumpRendererInfo() const;
        void dumpDisplayModes() const;
        void dumpDesktopDisplayMode() const;

      private:
        SDL_Window * m_windowPtr;
        SDL_Renderer * m_rendererPtr;
    };

} // namespace castlecrawl

#endif // SDLMANAGER_HPP_INCLUDED
