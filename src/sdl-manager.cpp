// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// sdl-manager.cpp
//
#include "sdl-manager.hpp"

#include "check-macros.hpp"
#include "sdl-util.hpp"
#include "sfml-util.hpp"

#include <iostream>

#include <SFML/Graphics.hpp>

namespace castlecrawl
{
    SDLManager::SDLManager()
        : m_windowPtr(nullptr)
        , m_rendererPtr(nullptr)
    {}

    void SDLManager::setup(
        const std::string & title,
        const int width,
        const int height,
        const bool willVSync,
        const SDLManagerFlags & flags)
    {
        const int initResult = SDL_Init(flags.sdl_init);
        M_CHECK((initResult == 0), "SDL_Init() failed: " << SDL_GetError());

        // const int sdlImageInitFlagsActual = IMG_Init(flags.sdl_image_init);
        //
        // M_CHECK(
        //    ((flags.sdl_image_init & sdlImageInitFlagsActual) == flags.sdl_image_init),
        //    "IMG_Init() failed: " << IMG_GetError());

        m_windowPtr = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width,
            height,
            flags.window);

        M_CHECK((m_windowPtr != nullptr), "SDL_CreateWindow() failed: " << SDL_GetError());

        const unsigned rendererFlagsToUse =
            (flags.renderer | ((willVSync) ? SDL_RENDERER_PRESENTVSYNC : 0));

        m_rendererPtr = SDL_CreateRenderer(m_windowPtr, -1, rendererFlagsToUse);
        M_CHECK((m_rendererPtr != nullptr), "SDL_CreateRenderer() failed: " << SDL_GetError());
    }

    void SDLManager::teardown()
    {
        if (m_rendererPtr != nullptr)
        {
            SDL_DestroyRenderer(m_rendererPtr);
            m_rendererPtr = nullptr;
        }

        if (m_windowPtr != nullptr)
        {
            SDL_DestroyWindow(m_windowPtr);
            m_windowPtr = nullptr;
        }

        SDL_Quit();
    }

    void SDLManager::renderClear(const SDL_Color & color)
    {
        SDL_SetRenderDrawColor(m_rendererPtr, color.r, color.g, color.b, color.a);
        SDL_RenderClear(m_rendererPtr);
    }

    void SDLManager::renderPresent() { SDL_RenderPresent(m_rendererPtr); }

    void SDLManager::blit(
        SDL_Texture * const texturePtr, const SDL_Rect & srcRect, const SDL_Rect & destRect)
    {
        M_CHECK((texturePtr != nullptr), "Tried to blit() a nullptr!");
        SDL_RenderCopy(m_rendererPtr, texturePtr, &srcRect, &destRect);
    }

    void SDLManager::blit(SDL_Texture * const texturePtr, int x, int y)
    {
        M_CHECK((texturePtr != nullptr), "Tried to blit() a nullptr!");

        SDL_Rect dest{ x, y, 0, 0 };
        const int queryResult = SDL_QueryTexture(texturePtr, nullptr, nullptr, &dest.w, &dest.h);
        M_CHECK((queryResult == 0), "SDL_QueryTexture() failed: " << SDL_GetError());

        SDL_RenderCopy(m_rendererPtr, texturePtr, nullptr, &dest);
    }

    void SDLManager::blit(SDL_Texture * const texturePtr, const BlitRectsVec_t & rects)
    {
        M_CHECK((texturePtr != nullptr), "Tried to blit() a nullptr!");

        for (const BlitRects & rect : rects)
        {
            SDL_RenderCopy(m_rendererPtr, texturePtr, &rect.src, &rect.dest);
        }
    }

    void SDLManager::draw(const SDL_Color & color, const RectVec_t & rects)
    {
        SDL_SetRenderDrawBlendMode(m_rendererPtr, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(m_rendererPtr, color.r, color.g, color.b, color.a);

        for (const SDL_Rect & rect : rects)
        {
            SDL_RenderFillRect(m_rendererPtr, &rect);
        }
    }

    void SDLManager::draw(const SDL_Color & color, const SDL_Rect & rect)
    {
        SDL_SetRenderDrawBlendMode(m_rendererPtr, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(m_rendererPtr, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(m_rendererPtr, &rect);
    }

    void SDLManager::drawOutline(const SDL_Color & color, const SDL_Rect & rect)
    {
        SDL_SetRenderDrawBlendMode(m_rendererPtr, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(m_rendererPtr, color.r, color.g, color.b, color.a);
        SDL_RenderDrawRect(m_rendererPtr, &rect);
    }

    SDL_Texture * SDLManager::loadTexture(const std::string & path) const
    {
        //
        // using SDL_Image
        //
        // SDL_Texture * const texturePtr = IMG_LoadTexture(m_rendererPtr, path.c_str());
        // M_CHECK((texturePtr != nullptr), "IMG_LoadTexture() failed: " << IMG_GetError());
        // return texturePtr;

        //
        // using only SDL
        //
        // SDL_Surface * const surfacePtr = SDL_LoadBMP(path.c_str());
        // M_CHECK((surfacePtr != nullptr), "SDL_LoadBMP(" << path << ") failed: " <<
        // SDL_GetError());
        //
        // SDL_Texture * const texturePtr = SDL_CreateTextureFromSurface(m_rendererPtr, surfacePtr);
        //
        // M_CHECK(
        //    (texturePtr != nullptr), "SDL_CreateTextureFromSurface() failed: " << SDL_GetError());
        //
        // util::freeSurface(surfacePtr);
        //
        // return texturePtr;

        //
        // using both SFML and SDL
        //
        sf::Image sfmlImage;
        const bool sfmlLoadResult = sfmlImage.loadFromFile(path);

        M_CHECK(
            sfmlLoadResult, "While loadTexture(" << path << ") sf::Image::loadFromFile() failed!");

        const sf::Vector2i size{ sfmlImage.getSize() };

        SDL_Surface * surfacePtr =
            SDL_CreateRGBSurfaceWithFormat(0, size.x, size.y, 32, SDL_PIXELFORMAT_RGBA8888);

        M_CHECK(
            (surfacePtr != nullptr),
            "SDL_CreateRGBSurfaceWithFormat("
                << size.x << 'x' << size.y
                << ", 32bit, SDL_PIXELFORMAT_RGBA8888) failed: " << SDL_GetError());

        const int setBlendModeResult = SDL_SetSurfaceBlendMode(surfacePtr, SDL_BLENDMODE_BLEND);
        M_CHECK((setBlendModeResult == 0), "SDL_SetSurfaceBlendMode() failed: " << SDL_GetError());

        SDL_LockSurface(surfacePtr);

        for (int y(0); y < size.y; ++y)
        {
            for (int x(0); x < size.x; ++x)
            {
                setPixel(
                    surfacePtr,
                    x,
                    y,
                    sfmlImage.getPixel(static_cast<unsigned>(x), static_cast<unsigned>(y))
                        .toInteger());
            }
        }

        SDL_UnlockSurface(surfacePtr);

        SDL_Texture * const texturePtr = SDL_CreateTextureFromSurface(m_rendererPtr, surfacePtr);

        M_CHECK(
            (texturePtr != nullptr),
            "While loadTexture(" << path << ", size=" << size.x << 'x' << size.y
                                 << ") SDL_CreateTextureFromSurface() failed: " << SDL_GetError());

        util::freeSurface(surfacePtr);

        return texturePtr;
    }

    SDL_Texture * SDLManager::loadAndSmoothResizeTexture(
        const std::string & path, const SDL_Point & newSize) const
    {
        sf::Texture sfmlTexture;
        const bool sfmlLoadResult = sfmlTexture.loadFromFile(path);

        M_CHECK(
            sfmlLoadResult,
            "While loadAndSmoothResizeTexture(" << path << ") sf::Texture::loadFromFile() failed!");

        sfmlTexture.setSmooth(true);

        sf::Sprite sprite(sfmlTexture);

        util::fitAndCenterInside(
            sprite,
            sf::FloatRect{ { 0.0f, 0.0f },
                           { static_cast<float>(newSize.x), static_cast<float>(newSize.y) } });

        sf::RenderTexture renderTexture;

        const bool renderTextureCreateResult = renderTexture.create(
            static_cast<unsigned>(newSize.x), static_cast<unsigned>(newSize.y));

        M_CHECK(
            renderTextureCreateResult,
            "While loadAndSmoothResizeTexture(" << path << ") sf::RenderTexture::create("
                                                << newSize.x << 'x' << newSize.y << ") failed!");

        renderTexture.clear(sf::Color::Transparent);
        renderTexture.draw(sprite);
        renderTexture.display();

        const sf::Image sfmlImage = renderTexture.getTexture().copyToImage();

        SDL_Surface * surfacePtr = SDL_CreateRGBSurfaceWithFormat(
            0,
            static_cast<int>(newSize.x),
            static_cast<int>(newSize.y),
            32,
            SDL_PIXELFORMAT_RGBA8888);

        M_CHECK(
            (surfacePtr != nullptr),
            "While loadAndSmoothResizeTexture(\""
                << path << "\") SDL_CreateRGBSurfaceWithFormat(" << newSize.x << 'x' << newSize.y
                << ", 32bit, SDL_PIXELFORMAT_RGBA8888) failed: " << SDL_GetError());

        const int setBlendModeResult = SDL_SetSurfaceBlendMode(surfacePtr, SDL_BLENDMODE_BLEND);

        M_CHECK(
            (setBlendModeResult == 0),
            "While loadAndSmoothResizeTexture(\""
                << path << "\") SDL_SetSurfaceBlendMode() failed: " << SDL_GetError());

        SDL_LockSurface(surfacePtr);

        for (int y(0); y < newSize.y; ++y)
        {
            for (int x(0); x < newSize.x; ++x)
            {
                setPixel(surfacePtr, x, y, sfmlImage.getPixel(x, y).toInteger());
            }
        }

        SDL_UnlockSurface(surfacePtr);

        SDL_Texture * const texturePtr = SDL_CreateTextureFromSurface(m_rendererPtr, surfacePtr);

        M_CHECK(
            (texturePtr != nullptr),
            "While loadAndSmoothResizeTexture(\"" << path << "\") SDL_CreateTextureFromSurface("
                                                  << newSize.x << 'x' << newSize.y
                                                  << ") failed: " << SDL_GetError());

        util::freeSurface(surfacePtr);

        return texturePtr;
    }

    SDL_Texture *
        SDLManager::loadAndMaskTexture(const std::string & path, const SDL_Color & maskColor) const
    {
        SDL_Surface * surfacePtr = SDL_LoadBMP(path.c_str());
        M_CHECK((surfacePtr != nullptr), "SDL_LoadBMP(" << path << ") failed: " << SDL_GetError());

        SDL_Surface * convSurfacePtr =
            SDL_ConvertSurfaceFormat(surfacePtr, SDL_PIXELFORMAT_RGBA8888, 0);

        M_CHECK(
            (convSurfacePtr != nullptr),
            "While loadAndMaskTexture("
                << path << ") SDL_ConvertSurfaceFormat() failed: " << SDL_GetError());

        util::freeSurface(surfacePtr);

        const int setBlendModeResult = SDL_SetSurfaceBlendMode(convSurfacePtr, SDL_BLENDMODE_BLEND);
        M_CHECK(
            (setBlendModeResult == 0),
            "While loadAndMaskTexture("
                << path << ") SDL_SetSurfaceBlendMode() failed: " << SDL_GetError());

        SDL_LockSurface(convSurfacePtr);

        for (int y(0); y < convSurfacePtr->h; ++y)
        {
            for (int x(0); x < convSurfacePtr->w; ++x)
            {
                if (getPixel(convSurfacePtr, x, y) == maskColor)
                {
                    setPixel(convSurfacePtr, x, y, 0x00000000);
                }
            }
        }

        SDL_UnlockSurface(convSurfacePtr);

        SDL_Texture * const texturePtr =
            SDL_CreateTextureFromSurface(m_rendererPtr, convSurfacePtr);

        M_CHECK(
            (texturePtr != nullptr),
            "While loadAndMaskTexture("
                << path << ") SDL_CreateTextureFromSurface() failed: " << SDL_GetError());

        util::freeSurface(convSurfacePtr);

        return texturePtr;
    }

    const SDL_Color
        SDLManager::getPixel(SDL_Surface * const surfacePtr, const int x, const int y) const
    {
        M_CHECK((surfacePtr != nullptr), "Tried to getPixel() from a nullptr!");

        const int bpp = surfacePtr->format->BytesPerPixel;

        const Uint8 * pixelPtr = static_cast<Uint8 *>(surfacePtr->pixels) +
                                 static_cast<std::ptrdiff_t>(y * surfacePtr->pitch + x * bpp);

        Uint32 data = *(Uint32 *)pixelPtr;

        if (1 == bpp)
        {
            data = *pixelPtr;
        }
        else if (2 == bpp)
        {
            data = *(Uint16 *)pixelPtr;
        }
        else if (3 == bpp)
        {
            if constexpr (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                data = static_cast<Uint32>(pixelPtr[0] << 16 | pixelPtr[1] << 8 | pixelPtr[2]);
            }
            else
            {
                data = static_cast<Uint32>(pixelPtr[0] | pixelPtr[1] << 8 | pixelPtr[2] << 16);
            }
        }

        SDL_Color rgb{ 0, 0, 0, 0 };
        SDL_GetRGBA(data, surfacePtr->format, &rgb.r, &rgb.g, &rgb.b, &rgb.a);
        return rgb;
    }

    void SDLManager::setPixel(
        SDL_Surface * surfacePtr, const int x, const int y, const unsigned color) const
    {
        M_CHECK((surfacePtr != nullptr), "Tried to setPixel() with a nullptr!");

        Uint32 * pixelPtr = (Uint32 *)(
            static_cast<Uint8 *>(surfacePtr->pixels) +
            static_cast<std::ptrdiff_t>(
                y * surfacePtr->pitch + x * surfacePtr->format->BytesPerPixel));

        *pixelPtr = color;
    }

    SDL_Texture * SDLManager::makeTextTexture(const sf::Text & sfmlText)
    {
        M_CHECK(!sfmlText.getString().isEmpty(), "Tried to create an empty string!");

        const sf::Vector2u size{ util::size(sfmlText.getGlobalBounds()) };

        M_CHECK(
            ((size.x > 0) && (size.y > 0)),
            "While makeTextTexture(\"" << sfmlText.getString().toAnsiString()
                                       << "\") Tried to create an sf::Text of size too small! ("
                                       << size.x << 'x' << size.y << ")");

        sf::RenderTexture renderTexture;
        const bool renderTextureCreateResult = renderTexture.create(size.x, size.y);

        M_CHECK(
            renderTextureCreateResult,
            "While makeTextTexture(\"" << sfmlText.getString().toAnsiString()
                                       << "\") sf::RenderTexture::create(" << size.x << 'x'
                                       << size.y << ") failed!");

        // If you draw text to a transparent texture using BlendAlpha the result will be ugly/fuzzy.
        // No clue why and the sfml forums don't know either...
        // So use BlendNone to avoid this problem.
        sf::RenderStates states;
        states.blendMode = sf::BlendNone;

        renderTexture.clear(sf::Color::Transparent);
        renderTexture.draw(sfmlText, states);
        renderTexture.display();

        const sf::Image sfmlImage = renderTexture.getTexture().copyToImage();

        SDL_Surface * surfacePtr = SDL_CreateRGBSurfaceWithFormat(
            0, static_cast<int>(size.x), static_cast<int>(size.y), 32, SDL_PIXELFORMAT_RGBA8888);

        M_CHECK(
            (surfacePtr != nullptr),
            "While makeTextTexture(\""
                << sfmlText.getString().toAnsiString() << "\") SDL_CreateRGBSurfaceWithFormat("
                << size.x << 'x' << size.y
                << ", 32bit, SDL_PIXELFORMAT_RGBA8888) failed: " << SDL_GetError());

        const int setBlendModeResult = SDL_SetSurfaceBlendMode(surfacePtr, SDL_BLENDMODE_BLEND);
        M_CHECK(
            (setBlendModeResult == 0),
            "While makeTextTexture(\""
                << sfmlText.getString().toAnsiString()
                << "\") SDL_SetSurfaceBlendMode() failed: " << SDL_GetError());

        SDL_LockSurface(surfacePtr);

        for (unsigned y(0); y < size.y; ++y)
        {
            for (unsigned x(0); x < size.x; ++x)
            {
                setPixel(
                    surfacePtr,
                    static_cast<int>(x),
                    static_cast<int>(y),
                    sfmlImage.getPixel(x, y).toInteger());
            }
        }

        SDL_UnlockSurface(surfacePtr);

        SDL_Texture * const texturePtr = SDL_CreateTextureFromSurface(m_rendererPtr, surfacePtr);

        M_CHECK(
            (texturePtr != nullptr),
            "While makeTextTexture(\"" << sfmlText.getString().toAnsiString()
                                       << "\") SDL_CreateTextureFromSurface(" << size.x << 'x'
                                       << size.y << ") failed: " << SDL_GetError());

        util::freeSurface(surfacePtr);

        return texturePtr;
    }

    void SDLManager::dumpDisplayDrivers() const
    {
        std::cout << "Display Drivers:\n";
        for (int i(0); i < SDL_GetNumRenderDrivers(); ++i)
        {
            SDL_RendererInfo info;
            if (SDL_GetRenderDriverInfo(i, &info) == 0)
            {
                std::cout << '\t' << info << '\n';
            }
        }
    }

    void SDLManager::dumpRendererInfo() const
    {
        SDL_RendererInfo info;
        SDL_GetRendererInfo(m_rendererPtr, &info);
        std::cout << "Current Renderer: " << info << std::endl;
    }

    void SDLManager::dumpDisplayModes() const
    {
        std::cout << "Display Modes:\n";
        for (int i(0); i < SDL_GetNumDisplayModes(0); ++i)
        {
            SDL_DisplayMode mode;
            const int displayModeResult = SDL_GetDisplayMode(0, i, &mode);

            M_CHECK(
                (displayModeResult == 0),
                "SDL_GetDisplayMode(0, " << i << ") failed: " << SDL_GetError());

            std::cout << '\t' << mode << '\n';
        }
    }

    void SDLManager::dumpDesktopDisplayMode() const
    {
        SDL_DisplayMode desktopMode;
        const int getDesktopModeResult = SDL_GetDesktopDisplayMode(0, &desktopMode);

        M_CHECK(
            (getDesktopModeResult == 0), "SDL_GetDesktopDisplayMode(0) failed: " << SDL_GetError());

        std::cout << "Desktop display mode is " << desktopMode << std::endl;
    }

} // namespace castlecrawl
