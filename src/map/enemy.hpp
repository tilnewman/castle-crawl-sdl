#ifndef ENEMY_HPP_INCLUDED
#define ENEMY_HPP_INCLUDED
//
// enemy.hpp
//
#include "display/tile-image-enum.hpp"
#include "map/map-types.hpp"
#include "misc/random.hpp"

#include <SDL.h>

#include <vector>

namespace castlecrawl
{
    struct Context;

    enum class Enemy
    {
        Snake,
        SnakeBag,
        Spider,
        Spiderweb,
        Goblin,
        GoblinBarrel,
        Bat,
        BatMask,
        Skeleton,
        SkeletonGrave,
        Demon,
        DemonDoor,
        Dragon,
        DragonInferno,
        Wizard,
        WizardTomb,
        Pixie,
        PixieCup
    };

    inline constexpr float walkToPlayerRatio(const Enemy enemy) noexcept
    {
        // clang-format off
        switch (enemy)
        {
            
            case Enemy::Snake:          { return 0.5f; }
            case Enemy::Spider:         { return 0.5f; }
            case Enemy::Goblin:         { return 0.8f; }
            case Enemy::Bat:            { return 0.3f; }
            case Enemy::Skeleton:       { return 0.9f; }
            case Enemy::Demon:          { return 1.0f; }
            case Enemy::Dragon:         { return 1.0f; }
            case Enemy::Wizard:         { return 0.8f; }
            case Enemy::Pixie:          { return 0.5f; }
            case Enemy::SnakeBag:       
            case Enemy::Spiderweb:      
            case Enemy::GoblinBarrel:   
            case Enemy::BatMask:        
            case Enemy::SkeletonGrave:  
            case Enemy::DemonDoor:
            case Enemy::WizardTomb:
            case Enemy::PixieCup:
            case Enemy::DragonInferno:
            default:                    { return 0.0; }
        }
        // clang-format on
    }

    inline float turnTimeSec(const util::Random & random, const Enemy enemy) noexcept
    {
        const float turnTimeSec = [&]() {
            // clang-format off
            switch (enemy)
            {
                case Enemy::Snake:          { return 2.0f; }
                case Enemy::SnakeBag:       { return 20.0f; }
                case Enemy::Spider:         { return 2.5f; }
                case Enemy::Spiderweb:      { return 25.0f; }
                case Enemy::Goblin:         { return 3.5f; }
                case Enemy::GoblinBarrel:   { return 35.0f; }
                case Enemy::Bat:            { return 1.5f; }
                case Enemy::BatMask:        { return 15.0f; }
                case Enemy::Skeleton:       { return 4.0f; }
                case Enemy::SkeletonGrave:  { return 40.0f; }
                case Enemy::Demon:          { return 4.5f; }
                case Enemy::DemonDoor:      { return 45.0f; }
                case Enemy::Dragon:         { return 5.0f; }
                case Enemy::DragonInferno:  { return 50.0f; }
                case Enemy::Wizard:         { return 6.0f; }
                case Enemy::WizardTomb:     { return 60.0f; }
                case Enemy::Pixie:          { return 3.0f; }
                case Enemy::PixieCup:       { return 45.0f; }
                default:                    { return 0.25f; } // error case easy to see during testing
            }
            // clang-format on
        }();

        const float range = random.zeroTo(0.3f * turnTimeSec);

        if (random.boolean())
        {
            return (turnTimeSec - range);
        }
        else
        {
            return (turnTimeSec + range);
        }
    }

    inline constexpr bool isEnemySummoner(const Enemy enemy) noexcept
    {
        // clang-format off
        switch (enemy)
        {
            case Enemy::SnakeBag:       
            case Enemy::Spiderweb:      
            case Enemy::GoblinBarrel:   
            case Enemy::BatMask:        
            case Enemy::SkeletonGrave:  
            case Enemy::DemonDoor:
            case Enemy::WizardTomb:
            case Enemy::PixieCup:
            case Enemy::DragonInferno:  { return true; }
            case Enemy::Snake:          
            case Enemy::Spider:         
            case Enemy::Goblin:         
            case Enemy::Bat:            
            case Enemy::Skeleton:       
            case Enemy::Demon:          
            case Enemy::Dragon:   
            case Enemy::Wizard:
            case Enemy::Pixie:
            default:                    { return false; }
        }
        // clang-format on
    }

    enum class EnemyImage : std::size_t
    {
        SnakeBag = 0,
        Snake1,
        Snake2,
        Snake3,
        Snake4,
        Snake5,
        Snake6,
        Spiderweb,
        Spider1,
        Spider2,
        Spider3,
        Spider4,
        Spider5,
        Spider6,
        GoblinBarrel,
        Goblin1,
        Goblin2,
        Goblin3,
        Goblin4,
        Goblin5,
        Goblin6,
        BatMask,
        Bat1,
        Bat2,
        Bat3,
        Bat4,
        Bat5,
        Bat6,
        PixieCup,
        Pixie1,
        Pixie2,
        Pixie3,
        Pixie4,
        Pixie5,
        Pixie6,
        SkeletonGrave,
        Skeleton1,
        Skeleton2,
        Skeleton3,
        Skeleton4,
        Skeleton5,
        Skeleton6,
        DemonDoor,
        Demon1,
        Demon2,
        Demon3,
        Demon4,
        Demon5,
        Demon6,
        DragonInferno,
        Dragon1,
        Dragon2,
        Dragon3,
        Dragon4,
        Dragon5,
        Dragon6,
        WizardTomb,
        Wizard1,
        Wizard2,
        Wizard3,
        Wizard4,
        Wizard5,
        Wizard6,
        FireLord
    };

    inline constexpr EnemyImage defaultEnemyImage(const Enemy enemy) noexcept
    {
        // clang-format off
        switch (enemy)
        {
            case Enemy::Snake:          { return EnemyImage::Snake1; }
            case Enemy::SnakeBag:       { return EnemyImage::SnakeBag; }
            case Enemy::Spider:         { return EnemyImage::Spider1; }
            case Enemy::Spiderweb:      { return EnemyImage::Spiderweb; }
            case Enemy::Goblin:         { return EnemyImage::Goblin1; }
            case Enemy::GoblinBarrel:   { return EnemyImage::GoblinBarrel; }
            case Enemy::Bat:            { return EnemyImage::Bat1; }
            case Enemy::BatMask:        { return EnemyImage::BatMask; }
            case Enemy::Skeleton:       { return EnemyImage::Skeleton1; }
            case Enemy::SkeletonGrave:  { return EnemyImage::SkeletonGrave; }
            case Enemy::Demon:          { return EnemyImage::Demon1; }
            case Enemy::DemonDoor:      { return EnemyImage::DemonDoor; }
            case Enemy::Dragon:         { return EnemyImage::Dragon1; }
            case Enemy::DragonInferno:  { return EnemyImage::DragonInferno; }
            case Enemy::Wizard:         { return EnemyImage::Wizard1; }
            case Enemy::WizardTomb:     { return EnemyImage::WizardTomb; }
            case Enemy::Pixie:          { return EnemyImage::Pixie1; }
            case Enemy::PixieCup:       { return EnemyImage::PixieCup; }
            default:                    { return EnemyImage::FireLord; }
        }
        // clang-format on
    }

    inline constexpr bool isEnemyImageSummoner(const EnemyImage enemyImage) noexcept
    {
        // clang-format off
        switch (enemyImage)
        {
            case EnemyImage::Snake1:        
            case EnemyImage::Snake2:        
            case EnemyImage::Snake3:        
            case EnemyImage::Snake4:        
            case EnemyImage::Snake5:        
            case EnemyImage::Snake6:        
            case EnemyImage::Spider1:       
            case EnemyImage::Spider2:       
            case EnemyImage::Spider3:       
            case EnemyImage::Spider4:       
            case EnemyImage::Spider5:       
            case EnemyImage::Spider6:       
            case EnemyImage::Goblin1:       
            case EnemyImage::Goblin2:       
            case EnemyImage::Goblin3:       
            case EnemyImage::Goblin4:       
            case EnemyImage::Goblin5:       
            case EnemyImage::Goblin6:       
            case EnemyImage::Bat1:          
            case EnemyImage::Bat2:          
            case EnemyImage::Bat3:          
            case EnemyImage::Bat4:          
            case EnemyImage::Bat5:          
            case EnemyImage::Bat6:          
            case EnemyImage::Pixie1:        
            case EnemyImage::Pixie2:        
            case EnemyImage::Pixie3:        
            case EnemyImage::Pixie4:        
            case EnemyImage::Pixie5:        
            case EnemyImage::Pixie6:        
            case EnemyImage::Skeleton1:     
            case EnemyImage::Skeleton2:     
            case EnemyImage::Skeleton3:     
            case EnemyImage::Skeleton4:     
            case EnemyImage::Skeleton5:     
            case EnemyImage::Skeleton6:     
            case EnemyImage::Demon1:        
            case EnemyImage::Demon2:        
            case EnemyImage::Demon3:        
            case EnemyImage::Demon4:        
            case EnemyImage::Demon5:        
            case EnemyImage::Demon6:        
            case EnemyImage::Dragon1:       
            case EnemyImage::Dragon2:       
            case EnemyImage::Dragon3:       
            case EnemyImage::Dragon4:       
            case EnemyImage::Dragon5:       
            case EnemyImage::Dragon6:       
            case EnemyImage::Wizard1:       
            case EnemyImage::Wizard2:       
            case EnemyImage::Wizard3:       
            case EnemyImage::Wizard4:       
            case EnemyImage::Wizard5:       
            case EnemyImage::Wizard6:       { return false; }
            case EnemyImage::Spiderweb:     
            case EnemyImage::GoblinBarrel:  
            case EnemyImage::SnakeBag:      
            case EnemyImage::DemonDoor:     
            case EnemyImage::BatMask:       
            case EnemyImage::PixieCup:      
            case EnemyImage::SkeletonGrave: 
            case EnemyImage::DragonInferno: 
            case EnemyImage::WizardTomb:    
            case EnemyImage::FireLord:
            default:                        { return true; }  
        }
        // clang-format on
    }

    inline const SDL_Rect enemyImageRect(const EnemyImage enemyImage) noexcept
    {
        // clang-format off
        switch (enemyImage)
        {
            case EnemyImage::SnakeBag:      { return {   0,   0, 64, 64}; }
            case EnemyImage::Snake1:        { return {  64,   0, 64, 64}; }
            case EnemyImage::Snake2:        { return { 128,   0, 64, 64}; }
            case EnemyImage::Snake3:        { return { 192,   0, 64, 64}; }
            case EnemyImage::Snake4:        { return { 256,   0, 64, 64}; }
            case EnemyImage::Snake5:        { return { 320,   0, 64, 64}; }
            case EnemyImage::Snake6:        { return { 384,   0, 64, 64}; }
            case EnemyImage::Spiderweb:     { return {   0,  64, 64, 64}; }
            case EnemyImage::Spider1:       { return {  64,  64, 64, 64}; }
            case EnemyImage::Spider2:       { return { 128,  64, 64, 64}; }
            case EnemyImage::Spider3:       { return { 192,  64, 64, 64}; }
            case EnemyImage::Spider4:       { return { 256,  64, 64, 64}; }
            case EnemyImage::Spider5:       { return { 320,  64, 64, 64}; }
            case EnemyImage::Spider6:       { return { 384,  64, 64, 64}; }
            case EnemyImage::GoblinBarrel:  { return {   0, 128, 64, 64}; }
            case EnemyImage::Goblin1:       { return {  64, 128, 64, 64}; }
            case EnemyImage::Goblin2:       { return { 128, 128, 64, 64}; }
            case EnemyImage::Goblin3:       { return { 192, 128, 64, 64}; }
            case EnemyImage::Goblin4:       { return { 256, 128, 64, 64}; }
            case EnemyImage::Goblin5:       { return { 320, 128, 64, 64}; }
            case EnemyImage::Goblin6:       { return { 384, 128, 64, 64}; }
            case EnemyImage::BatMask:       { return {   0, 192, 64, 64}; }
            case EnemyImage::Bat1:          { return {  64, 192, 64, 64}; }
            case EnemyImage::Bat2:          { return { 128, 192, 64, 64}; }
            case EnemyImage::Bat3:          { return { 192, 192, 64, 64}; }
            case EnemyImage::Bat4:          { return { 256, 192, 64, 64}; }
            case EnemyImage::Bat5:          { return { 320, 192, 64, 64}; }
            case EnemyImage::Bat6:          { return { 384, 192, 64, 64}; }
            case EnemyImage::PixieCup:      { return {   0, 256, 64, 64}; }
            case EnemyImage::Pixie1:        { return {  64, 256, 64, 64}; }
            case EnemyImage::Pixie2:        { return { 128, 256, 64, 64}; }
            case EnemyImage::Pixie3:        { return { 192, 256, 64, 64}; }
            case EnemyImage::Pixie4:        { return { 256, 256, 64, 64}; }
            case EnemyImage::Pixie5:        { return { 320, 256, 64, 64}; }
            case EnemyImage::Pixie6:        { return { 384, 256, 64, 64}; }
            case EnemyImage::SkeletonGrave: { return {   0, 320, 64, 64}; }
            case EnemyImage::Skeleton1:     { return {  64, 320, 64, 64}; }
            case EnemyImage::Skeleton2:     { return { 128, 320, 64, 64}; }
            case EnemyImage::Skeleton3:     { return { 192, 320, 64, 64}; }
            case EnemyImage::Skeleton4:     { return { 256, 320, 64, 64}; }
            case EnemyImage::Skeleton5:     { return { 320, 320, 64, 64}; }
            case EnemyImage::Skeleton6:     { return { 384, 320, 64, 64}; }
            case EnemyImage::DemonDoor:     { return {   0, 384, 64, 64}; }
            case EnemyImage::Demon1:        { return {  64, 384, 64, 64}; }
            case EnemyImage::Demon2:        { return { 128, 384, 64, 64}; }
            case EnemyImage::Demon3:        { return { 192, 384, 64, 64}; }
            case EnemyImage::Demon4:        { return { 256, 384, 64, 64}; }
            case EnemyImage::Demon5:        { return { 320, 384, 64, 64}; }
            case EnemyImage::Demon6:        { return { 384, 384, 64, 64}; }
            case EnemyImage::DragonInferno: { return {   0, 448, 64, 64}; }
            case EnemyImage::Dragon1:       { return {  64, 448, 64, 64}; }
            case EnemyImage::Dragon2:       { return { 128, 448, 64, 64}; }
            case EnemyImage::Dragon3:       { return { 192, 448, 64, 64}; }
            case EnemyImage::Dragon4:       { return { 256, 448, 64, 64}; }
            case EnemyImage::Dragon5:       { return { 320, 448, 64, 64}; }
            case EnemyImage::Dragon6:       { return { 384, 448, 64, 64}; }
            case EnemyImage::WizardTomb:    { return { 448,   0, 64, 64}; }
            case EnemyImage::Wizard1:       { return { 448,  64, 64, 64}; }
            case EnemyImage::Wizard2:       { return { 448, 128, 64, 64}; }
            case EnemyImage::Wizard3:       { return { 448, 192, 64, 64}; }
            case EnemyImage::Wizard4:       { return { 448, 256, 64, 64}; }
            case EnemyImage::Wizard5:       { return { 448, 320, 64, 64}; }
            case EnemyImage::Wizard6:       { return { 448, 384, 64, 64}; }
            case EnemyImage::FireLord:
            default:                        { return { 224, 224, 64, 64}; }
        }
        // clang-format on
    }

    EnemyImage randomEnemyImage(const util::Random & random, const Enemy enemy);

    inline constexpr Enemy spawnType(const Enemy summoner) noexcept
    {
        if (summoner == Enemy::BatMask)
        {
            return Enemy::Bat;
        }
        else if (summoner == Enemy::DemonDoor)
        {
            return Enemy::Demon;
        }
        else if (summoner == Enemy::DragonInferno)
        {
            return Enemy::Dragon;
        }
        else if (summoner == Enemy::GoblinBarrel)
        {
            return Enemy::Goblin;
        }
        else if (summoner == Enemy::SkeletonGrave)
        {
            return Enemy::Skeleton;
        }
        else if (summoner == Enemy::SnakeBag)
        {
            return Enemy::Snake;
        }
        else if (summoner == Enemy::WizardTomb)
        {
            return Enemy::Wizard;
        }
        else if (summoner == Enemy::PixieCup)
        {
            return Enemy::Pixie;
        }
        else
        {
            return Enemy::Spider;
        }
    }

    struct EnemyInstance
    {
        EnemyInstance(const util::Random & random, const Enemy e, const MapPos_t & p);

        bool isSummoner() const { return isEnemySummoner(enemy); }

        Enemy enemy;
        EnemyImage image;
        MapPos_t position;
        float timer_sec;
        float walk_toward_ratio;
    };

    class Enemies
    {
      public:
        Enemies();

        void setup(const Context & context);
        void teardown();
        void update(const Context & context, const float frameTimeSec);
        void add(const EnemyInstance & enemy);
        void remove(const MapPos_t & position);
        void removeAll() { m_enemies.clear(); }
        void draw(const Context & context) const;
        bool isAnyAtMapPos(const MapPos_t & pos) const;
        bool isAnyNonSummoner() const;
        void takeTurns(const Context & context);

      private:
        void act(const Context & context, EnemyInstance & enemy);
        void move(const Context & context, EnemyInstance & enemy);
        void spawn(const Context & context, EnemyInstance & enemy);
        void attack(const Context & context, EnemyInstance & enemy);

        void removeSpawnAndMoveObstacles(
            const Context & context, std::vector<MapCell> & cells) const;

      private:
        SDL_Texture * m_texturePtr;
        std::vector<EnemyInstance> m_enemies;
    };

} // namespace castlecrawl

#endif // ENEMY_HPP_INCLUDED
