#ifndef CONTEXT_HPP_INCLUDED
#define CONTEXT_HPP_INCLUDED
//
// context.hpp
//

namespace util
{
    class Random;
    class SoundPlayer;
    class MusicPlayer;
    class AnimationPlayer;
} // namespace util

namespace castlecrawl
{
    class SDLManager;
    struct GameConfig;
    class TileImages;
    class Layout;
    class Map;
    class Maps;
    class MapDisplay;
    class StateManager;
    class Player;
    class FontManager;
    class Enemies;
    class MapObjects;
    class FramerateText;
    class TopPanel;
    class TurnKeeper;

    struct Context
    {
        Context(
            SDLManager & sdlManagerParam,
            const GameConfig & configParam,
            const TileImages & tileImagesParam,
            Layout & layoutParam,
            Map & mapParam,
            Maps & mapsParam,
            MapDisplay & mapDisplayParam,
            StateManager & stateManagerParam,
            Player & playerParam,
            const util::Random & randomParam,
            util::SoundPlayer & sfxParam,
            util::MusicPlayer & musicParam,
            util::AnimationPlayer & animParam,
            FontManager & fontManagerParam,
            Enemies & enemiesParam,
            MapObjects & mapObjectsParam,
            FramerateText & framerateTextParam,
            TopPanel & topPanelParam,
            TurnKeeper & turnKeeperParam)
            : sdl(sdlManagerParam)
            , config(configParam)
            , tile_image(tileImagesParam)
            , layout(layoutParam)
            , map(mapParam)
            , maps(mapsParam)
            , map_display(mapDisplayParam)
            , state(stateManagerParam)
            , player(playerParam)
            , random(randomParam)
            , sfx(sfxParam)
            , music(musicParam)
            , anim(animParam)
            , font(fontManagerParam)
            , enemy(enemiesParam)
            , map_object(mapObjectsParam)
            , framerate(framerateTextParam)
            , top_panel(topPanelParam)
            , turn(turnKeeperParam)
        {}

        SDLManager & sdl;
        const GameConfig & config;
        const TileImages & tile_image;
        Layout & layout;
        Map & map;
        const Maps & maps;
        MapDisplay & map_display;
        StateManager & state;
        Player & player;
        const util::Random & random;
        util::SoundPlayer & sfx;
        util::MusicPlayer & music;
        util::AnimationPlayer & anim;
        FontManager & font;
        Enemies & enemy;
        MapObjects & map_object;
        FramerateText & framerate;
        TopPanel & top_panel;
        TurnKeeper & turn;
    };

} // namespace castlecrawl

#endif // CONTEXT_HPP_INCLUDED
