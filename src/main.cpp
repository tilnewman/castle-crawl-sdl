// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// main.cpp
//
#include <cstddef>

#include "game-config.hpp"
#include "loop-coordinator.hpp"

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char * argv[])
{
    castlecrawl::GameConfig config;

    const std::vector<std::string> args(argv, (argv + argc));
    if (args.size() < 2)
    {
        config.media_path = std::filesystem::path{ "media" };
    }
    else
    {
        config.media_path = std::filesystem::canonical({ args.at(1) });
    }

    if (std::filesystem::exists(config.media_path))
    {
        castlecrawl::LoopCoordinator coordinator(config);
        coordinator.playGame();
    }
    else
    {
        std::cout << "Unable to find the media directory at " << config.media_path.string()
                  << std::endl;
    }

    return EXIT_SUCCESS;
}
