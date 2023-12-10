// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// enemy.cpp
//
#include "enemy.hpp"

#include "check-macros.hpp"
#include "context.hpp"
#include "layout.hpp"
#include "map.hpp"
#include "player-display.hpp"
#include "random.hpp"
#include "sdl-manager.hpp"
#include "sdl-util.hpp"
#include "tile-images.hpp"

#include <type_traits>

namespace castlecrawl
{

    EnemyImage randomEnemyImage(const util::Random & random, const Enemy enemy)
    {
        EnemyImage enemyImage = defaultEnemyImage(enemy);

        if (!isEnemyImageSummoner(enemyImage))
        {
            using T = std::underlying_type_t<EnemyImage>;
            enemyImage = static_cast<EnemyImage>(T(enemyImage) + random.zeroTo(T(5)));
        }

        return enemyImage;
    }

    EnemyInstance::EnemyInstance(const util::Random & random, const Enemy e, const MapPos_t & p)
        : enemy(e)
        , image(randomEnemyImage(random, e))
        , position(p)
        , timer_sec(turnTimeSec(random, e))
        , walk_toward_ratio(walkToPlayerRatio(e))
    {}

    Enemies::Enemies()
        : m_texturePtr(nullptr)
        , m_enemies()
    {}

    void Enemies::setup(const Context & context)
    {
        m_texturePtr =
            context.sdl.loadTexture((context.config.media_path / "image" / "enemies.png").string());
    }

    void Enemies::teardown() { util::destroyTexture(m_texturePtr); }

    void Enemies::update(const Context & context, const float frameTimeSec)
    {
        for (EnemyInstance & enemy : m_enemies)
        {
            enemy.timer_sec -= frameTimeSec;
            if (enemy.timer_sec < 0.0f)
            {
                act(context, enemy);
            }
        }
    }

    void Enemies::add(const EnemyInstance & enemy) { m_enemies.push_back(enemy); }

    void Enemies::remove(const MapPos_t & pos)
    {
        m_enemies.erase(
            std::remove_if(
                std::begin(m_enemies),
                std::end(m_enemies),
                [&](const EnemyInstance & si) { return (si.position == pos); }),
            std::end(m_enemies));
    }

    void Enemies::draw(const Context & context) const
    {
        if (m_texturePtr == nullptr)
        {
            return;
        }

        for (const EnemyInstance & enemy : m_enemies)
        {
            const SDL_Rect srcRect = enemyImageRect(enemy.image);

            const SDL_Rect destRect = util::makeRect(
                context.map.mapPosToScreenPos(context, enemy.position), context.layout.cellSize());

            context.sdl.blit(m_texturePtr, srcRect, destRect);
        }
    }

    bool Enemies::isAnyAtMapPos(const MapPos_t & pos) const
    {
        for (const EnemyInstance & enemy : m_enemies)
        {
            if (enemy.position == pos)
            {
                return true;
            }
        }

        return false;
    }

    bool Enemies::isAnyNonSummoner() const
    {
        for (const EnemyInstance & enemy : m_enemies)
        {
            if (!enemy.isSummoner())
            {
                return true;
            }
        }

        return false;
    }

    void Enemies::takeTurns(const Context & context)
    {
        for (EnemyInstance & enemy : m_enemies)
        {
            if (!enemy.isSummoner())
            {
                act(context, enemy);
            }
        }
    }

    void Enemies::act(const Context & context, EnemyInstance & enemy)
    {
        enemy.timer_sec = turnTimeSec(context.random, enemy.enemy);

        if (enemy.isSummoner())
        {
            spawn(context, enemy);
        }
        else
        {
            if (context.player.isPosNextTo(enemy.position))
            {
                attack(context, enemy);
            }
            else
            {
                move(context, enemy);
            }
        }
    }

    void Enemies::move(const Context & context, EnemyInstance & enemy)
    {
        std::vector<MapCell> possibleMoveCells =
            context.map.surroundingCellsHorizVert(enemy.position);

        removeSpawnAndMoveObstacles(context, possibleMoveCells);

        if (possibleMoveCells.empty())
        {
            return;
        }

        if (context.random.fromTo(0.0f, 1.0f) < enemy.walk_toward_ratio)
        {
            auto distance = [](const MapPos_t & A, const MapPos_t & B) {
                return (util::abs(A.x - B.x) + util::abs(A.y - B.y));
            };

            std::sort(
                std::begin(possibleMoveCells),
                std::end(possibleMoveCells),
                [&](const MapCell & A, const MapCell & B) {
                    return (
                        distance(context.player.position(), A.position) <
                        distance(context.player.position(), B.position));
                });

            const int shortestDistance =
                distance(possibleMoveCells.front().position, context.player.position());

            possibleMoveCells.erase(
                std::remove_if(
                    std::begin(possibleMoveCells),
                    std::end(possibleMoveCells),
                    [&](const MapCell & cell) {
                        return (
                            distance(context.player.position(), cell.position) > shortestDistance);
                    }),
                std::end(possibleMoveCells));
        }

        enemy.position = context.random.from(possibleMoveCells).position;
    }

    void Enemies::spawn(const Context & context, EnemyInstance & enemy)
    {
        std::vector<MapCell> possibleMoveCells =
            context.map.surroundingCellsHorizVert(enemy.position);

        removeSpawnAndMoveObstacles(context, possibleMoveCells);

        if (possibleMoveCells.empty())
        {
            return;
        }

        const MapCell cellToMoveInto = context.random.from(possibleMoveCells);

        const EnemyInstance spawnedInstance(
            context.random, spawnType(enemy.enemy), cellToMoveInto.position);

        add(spawnedInstance);
    }

    void Enemies::attack(const Context &, EnemyInstance &)
    {
        // TODO
    }

    void Enemies::removeSpawnAndMoveObstacles(
        const Context & context, std::vector<MapCell> & cells) const
    {
        // can't move onto obstacles
        cells.erase(
            std::remove_if(
                std::begin(cells),
                std::end(cells),
                [&](const MapCell & cell) { return (cell.object_char != ' '); }),
            std::end(cells));

        // can't move onto player
        cells.erase(
            std::remove_if(
                std::begin(cells),
                std::end(cells),
                [&](const MapCell & cell) { return (cell.position == context.player.position()); }),
            std::end(cells));

        // can't move onto other enemies
        cells.erase(
            std::remove_if(
                std::begin(cells),
                std::end(cells),
                [&](const MapCell & cell) {
                    for (const EnemyInstance & e : m_enemies)
                    {
                        if (cell.position == e.position)
                        {
                            return true;
                        }
                    }

                    return false;
                }),
            std::end(cells));
    }

} // namespace castlecrawl
