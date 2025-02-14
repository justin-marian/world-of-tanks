#pragma once

#ifndef PROJECTILES_H
#define PROJECTILES_H

#include "EnemyTanks.h"
#include "Buildings.h"

#include <glm/glm.hpp>
#include <vector>


struct Building;
struct EnemyTank;
struct PlayerTank;


/// <summary>
/// Projectile fired by a tank.
/// </summary>
struct Projectile
{
    glm::vec3 position;         // Current position of the projectile.
    glm::vec3 velocity;         // Velocity of the projectile.

    float radius;               // Radius of the projectile.
    float lifespan = 0.0f;      // Current lifespan of the projectile.
    float maxLifespan = 1.0f;   // Maximum lifespan of the projectile (in seconds).
};


class Projectiles {
public:
    /// Check if a projectile has collided with an enemy tank
    static bool ProjectileTankCollision(
        const Projectile& projectile,
        const EnemyTank& tank
    );
    
    /// Check if a projectile has collided with the player tank
    static bool ProjectilePlayerCollision(
        const Projectile& projectile,
        const PlayerTank& player
    );

    /// Check if a projectile has collided with a building
    static bool ProjectileBuildingCollision(
        const Projectile& projectile,
        const Building& building
    );

    /// Update projectile collisions with the player tank
    static void UpdateProjectilesPlayerCollision(
        std::vector<Projectile>& projectiles,
        PlayerTank& player,
        int damage
    );

    /// Update projectile movements and collisions with enemy tanks
    static void UpdateProjectileMovementsAndCollisions(
        std::vector<Projectile>& projectiles,
        std::vector<EnemyTank>& enemies,
        int projectileDamage,
        float deltaTime
    );
};

#endif // PROJECTILES_H
