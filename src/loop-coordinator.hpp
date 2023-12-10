#ifndef LOOPCOORDINATOR_HPP_INCLUDED
#define LOOPCOORDINATOR_HPP_INCLUDED
//
// loop-coordinator.hpp
//
#include "animation-player.hpp"
#include "context.hpp"
#include "enemy.hpp"
#include "font.hpp"
#include "framerate-text.hpp"
#include "game-config.hpp"
#include "layout.hpp"
#include "map-display.hpp"
#include "map-objects.hpp"
#include "map.hpp"
#include "maps.hpp"
#include "music-player.hpp"
#include "player-display.hpp"
#include "random.hpp"
#include "sdl-manager.hpp"
#include "sound-player.hpp"
#include "state-manager.hpp"
#include "tile-images.hpp"
#include "top-panel.hpp"
#include "turn-keeper.hpp"

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
        Player m_player;
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

        Context m_context;
    };

} // namespace castlecrawl

#endif // LOOPCOORDINATOR_HPP_INCLUDED
