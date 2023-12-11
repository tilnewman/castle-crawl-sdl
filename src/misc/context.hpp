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
    namespace item
    {
        class ItemFactory;
    }

    class SDLManager;
    struct GameConfig;
    class TileImages;
    class Layout;
    class Map;
    class Maps;
    class MapDisplay;
    class StateManager;
    class PlayerDisplay;
    class FontManager;
    class Enemies;
    class MapObjects;
    class FramerateText;
    class TopPanel;
    class TurnKeeper;
    class Player;

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
            PlayerDisplay & playerDisplayParam,
            const util::Random & randomParam,
            util::SoundPlayer & sfxParam,
            util::MusicPlayer & musicParam,
            util::AnimationPlayer & animParam,
            FontManager & fontManagerParam,
            Enemies & enemiesParam,
            MapObjects & mapObjectsParam,
            FramerateText & framerateTextParam,
            TopPanel & topPanelParam,
            TurnKeeper & turnKeeperParam,
            Player & playerParam,
            item::ItemFactory & itemFactoryParam)
            : sdl(sdlManagerParam)
            , config(configParam)
            , tile_image(tileImagesParam)
            , layout(layoutParam)
            , map(mapParam)
            , maps(mapsParam)
            , map_display(mapDisplayParam)
            , state(stateManagerParam)
            , player_display(playerDisplayParam)
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
            , player(playerParam)
            , item_factory(itemFactoryParam)
        {}

        SDLManager & sdl;
        const GameConfig & config;
        const TileImages & tile_image;
        Layout & layout;
        Map & map;
        const Maps & maps;
        MapDisplay & map_display;
        StateManager & state;
        PlayerDisplay & player_display;
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
        Player & player;
        item::ItemFactory & item_factory;
    };

} // namespace castlecrawl

#endif // CONTEXT_HPP_INCLUDED
