#ifndef MAPTYPES_HPP_INCLUDED
#define MAPTYPES_HPP_INCLUDED
//
// map-types.hpp
//
#include <string>
#include <string_view>
#include <vector>

#include <SDL.h>

namespace castlecrawl
{

    struct BlitRects
    {
        SDL_Rect src;
        SDL_Rect dest;
    };

    using BlitRectsVec_t = std::vector<BlitRects>;
    using RectVec_t = std::vector<SDL_Rect>;

    using MapPos_t = SDL_Point;
    using MapChars_t = std::vector<std::string>;

    struct MapCell
    {
        // these default values are safe where needed and invalid where needed -don't change
        MapPos_t position = { -1, -1 };
        char object_char = '.';
        char floor_char = ' ';
    };

    using Map_t = std::vector<std::vector<MapCell>>;

    enum class Floor
    {
        Dirt,
        Stone,
        Wood
    };

    enum class MapName
    {
        Level_1_Cell,
        Level_1_CellBlock,
        Level_1_MainHall,
        Level_1_BoilerRoom,
        Level_1_HiddenPassage,
        Level_1_CoffinRoom,
        Level_1_LakeStronghold,
        Level_1_LavaGatehouse
    };

    inline constexpr std::string_view toString(const MapName name) noexcept
    {
        // clang-format off
        switch (name)
        {
            case MapName::Level_1_Cell:             { return "Level_1_Cell"; }
            case MapName::Level_1_CellBlock:        { return "Level_1_CellBlock"; }
            case MapName::Level_1_MainHall:         { return "Level_1_MainHall"; }
            case MapName::Level_1_BoilerRoom:       { return "Level_1_BoilerRoom"; }
            case MapName::Level_1_HiddenPassage:    { return "Level_1_HiddenPassage"; }
            case MapName::Level_1_CoffinRoom:       { return "Level_1_CoffinRoom"; }
            case MapName::Level_1_LakeStronghold:   { return "Level_1_LakeStronghold"; }
            case MapName::Level_1_LavaGatehouse:    { return "Level_1_LavaGatehouse"; }
            default:                                { return "(unknown_MapName)"; }
        }
        // clang-format on
    }

    inline std::ostream & operator<<(std::ostream & os, const MapName mapName)
    {
        os << toString(mapName);
        return os;
    }

    struct MapTransition
    {
        MapPos_t from_pos{ -1, -1 };              // both must be negative to be invalid
        MapName to_name{ MapName::Level_1_Cell }; // anything works here
        MapPos_t to_pos{ -1, -1 };                // both must be negative to be invalid
    };

    using MapTransitions_t = std::vector<MapTransition>;

} // namespace castlecrawl

#endif // MAPTYPES_HPP_INCLUDED
