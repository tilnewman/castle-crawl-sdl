// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// loop-coordinator.cpp
//
#include "loop-coordinator.hpp"

#include "check-macros.hpp"
#include "clock.hpp"
#include "sdl-util.hpp"
#include "tile-image-enum.hpp"

#include <exception>
#include <iostream>

namespace castlecrawl
{

    LoopCoordinator::LoopCoordinator(const GameConfig & config)
        : m_sdlManager()
        , m_config(config)
        , m_tileImages()
        , m_layout()
        , m_map()
        , m_maps()
        , m_mapDisplay()
        , m_player()
        , m_random()
        , m_sfx(m_random)
        , m_music()
        , m_fonts()
        , m_enemies()
        , m_framerate()
        , m_topPanel()
        , m_stateManager()
        , m_context(
              m_sdlManager,
              m_config,
              m_tileImages,
              m_layout,
              m_map,
              m_maps,
              m_mapDisplay,
              m_stateManager,
              m_player,
              m_random,
              m_sfx,
              m_music,
              m_fonts,
              m_enemies,
              m_framerate,
              m_topPanel)
    {}

    void LoopCoordinator::playGame()
    {
        try
        {
            setup();
            runLoop();
        }
        catch (const std::exception & ex)
        {
            std::cout << "Standard Exception: " << ex.what() << std::endl;
        }
        catch (...)
        {
            std::cout << "Unknown Exception: " << std::endl;
        }

        teardown();
    }

    void LoopCoordinator::setup()
    {
        m_sdlManager.setup(
            "Castle Crawl", m_config.resolution.x, m_config.resolution.y, m_config.will_vsync);

        // m_sdlManager.dumpDisplayDrivers();
        // m_sdlManager.dumpRendererInfo();
        // m_sdlManager.dumpDisplayModes();
        // m_sdlManager.dumpDesktopDisplayMode();

        m_sfx.setMediaPath((m_config.media_path / "sfx").string());
        m_sfx.loadAll();

        m_framerate.setup();
        m_fonts.setup(m_config);
        m_enemies.setup(m_context);
        m_tileImages.setup(m_context);
        m_layout.setup(m_config);
        m_maps.setup(m_context);
        m_topPanel.setup(m_context);

        m_stateManager.change(m_context, State::Splash);

        m_music.setup((m_config.media_path / "music").string());
        m_music.start("music.ogg", m_config.music_volume);

        // TODO move somewhere else, why is it even needed?
        m_maps.change(m_context, MapName::Level_1_Cell, { 3, 2 });
    }

    void LoopCoordinator::teardown()
    {
        m_sfx.stopAll();
        m_music.stopAll();
        m_framerate.teardown();
        m_tileImages.teardown();
        m_enemies.teardown();
        m_topPanel.teardown();
        m_sdlManager.teardown();
    }

    void LoopCoordinator::runLoop()
    {
        Clock frameClock;

        while (!m_context.is_game_over)
        {
            handleEvents();
            update(frameClock.restart());
            draw();
        }
    }

    void LoopCoordinator::handleEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            handleEvent(event);
        }
    }

    void LoopCoordinator::handleEvent(const SDL_Event & event)
    {
        if (SDL_QUIT == event.type)
        {
            m_context.is_game_over = true;
        }
        else if ((SDL_KEYUP == event.type) && (SDLK_ESCAPE == event.key.keysym.sym))
        {
            m_context.is_game_over = true;
        }
        else
        {
            m_stateManager.current().handleEvent(m_context, event);
        }
    }

    void LoopCoordinator::draw()
    {
        m_sdlManager.renderClear(m_config.background_color);
        m_stateManager.current().draw(m_context);
        m_sdlManager.renderPresent();
    }

    void LoopCoordinator::update(const float frameTimeSec)
    {
        m_stateManager.current().update(m_context, frameTimeSec);
    }

} // namespace castlecrawl
