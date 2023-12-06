// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// main.cpp
//
#include <cstddef>

#include "game-config.hpp"
#include "loop-coordinator.hpp"

int main(int, char *[])
{
    using namespace castlecrawl;

    GameConfig config;
    LoopCoordinator coordinator(config);
    coordinator.playGame();

    return EXIT_SUCCESS;
}
