#ifndef GAMECONFIG_HPP_INCLUDED
#define GAMECONFIG_HPP_INCLUDED
//
// game-config.hpp
//
#include <filesystem>

#include <SDL.h>

namespace castlecrawl
{

    struct GameConfig
    {
        std::filesystem::path media_path = { "media" };
        SDL_Point resolution = { 1920, 1200 };
        SDL_Color background_color = { 0, 0, 0, 255 };
        float music_volume = 40.0f;
        unsigned int framerate_limit = 0;
        float top_panel_height_ratio = 0.15f;
        SDL_Color image_mask_color = { 255, 0, 255, 255 };
        bool will_vsync = false;

        // DON'T CHANGE UNLESS YOU ALSO CHECK ALL MAPS IN THE GAME
        SDL_Point map_cell_size_max = { 41, 23 };
    };

} // namespace castlecrawl

#endif // GAMECONFIG_HPP_INCLUDED
