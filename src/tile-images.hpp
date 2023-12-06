#ifndef TILEIMAGES_HPP_INCLUDED
#define TILEIMAGES_HPP_INCLUDED
//
// tile-images.hpp
//
#include "tile-image-enum.hpp"

#include <SDL.h>

namespace castlecrawl
{
    struct Context;

    class TileImages
    {
      public:
        TileImages();

        void setup(const Context & context);
        void teardown();

        SDL_Texture * texturePtr() const { return m_texturePtr; }

      private:
        SDL_Texture * m_texturePtr;
    };

} // namespace castlecrawl

#endif // TILEIMAGES_HPP_INCLUDED
