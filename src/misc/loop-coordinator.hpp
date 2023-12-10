#ifndef LOOPCOORDINATOR_HPP_INCLUDED
#define LOOPCOORDINATOR_HPP_INCLUDED
//
// loop-coordinator.hpp
//
#include "display/animation-player.hpp"
#include "display/font.hpp"
#include "display/framerate-text.hpp"
#include "display/layout.hpp"
#include "display/sdl-manager.hpp"
#include "display/tile-images.hpp"
#include "display/top-panel.hpp"
#include "map/enemy.hpp"
#include "map/map-display.hpp"
#include "map/map-objects.hpp"
#include "map/map.hpp"
#include "map/maps.hpp"
#include "map/player-display.hpp"
#include "misc/context.hpp"
#include "misc/game-config.hpp"
#include "misc/music-player.hpp"
#include "misc/random.hpp"
#include "misc/sound-player.hpp"
#include "misc/turn-keeper.hpp"
#include "player/player.hpp"
#include "state/state-manager.hpp"

namespace castlecrawl
{

    class LoopCoordinator
    {
      public:
        LoopCoordinator(const GameConfig & config);

        void playGame();

      private:
        void setup();
        void teardown();
        void runLoop();
        void handleEvents();
        void handleEvent(const SDL_Event & event);
        void draw();
        void update(const float frameTimeSec);

      private:
        SDLManager m_sdlManager;
        GameConfig m_config;
        TileImages m_tileImages;
        Layout m_layout;
        Map m_map;
        Maps m_maps;
        MapDisplay m_mapDisplay;
        PlayerDisplay m_playerDisplay;
        util::Random m_random;
        util::SoundPlayer m_sfx;
        util::MusicPlayer m_music;
        util::AnimationPlayer m_anim;
        FontManager m_fonts;
        Enemies m_enemies;
        MapObjects m_mapObjects;
        FramerateText m_framerate;
        TopPanel m_topPanel;
        StateManager m_stateManager;
        TurnKeeper m_turnKeeper;
        Player m_player;

        Context m_context;
    };

} // namespace castlecrawl

#endif // LOOPCOORDINATOR_HPP_INCLUDED
