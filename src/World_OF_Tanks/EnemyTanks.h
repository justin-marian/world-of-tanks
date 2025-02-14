/// Defines the EnemyTank and PlayerTank classes
/// along with functions related to enemy tanks.

#pragma once

#ifndef ENEMY_TANKS_H
#define ENEMY_TANKS_H

#include "Buildings.h"
#include "Projectiles.h"

#include <glm/glm.hpp>
#include <vector>
#include <map>


struct Building;
struct Projectile;


/// <summary>
/// ENEMY TANKS THAT FOLLOW THE PLAYER TANK AND SHOOT HIM,
/// TRIES TO AVOID COLLISIONS WITH BUILDINGS
/// </summary>
struct EnemyTank
{
    glm::vec3 position;        // Position of the tank
    glm::vec3 direction;       // Direction the tank is facing

    float radius;              // Radius of the tank
    float rotation;            // Rotation angle of the tank's body
    float turretRotation;      // Rotation angle of the tank's turret

    int movementPattern;       // Pattern for tank's movement
    float movementTimer;       // Timer for movement pattern

    int health;                // Tank's health
    float sinkSpeed = 0.5;     // Speed tank sinks
    float sinkDepth = 0.0;     // Depth tank sinks

    bool isRenderable;         // Tank should be rendered
    bool isDestroyed = false;  // Tank is destroyed

    bool isPlayerInRange;      // Player is in range
    float timeSinceLastShot;   // Time elapsed since the last shot
    float deformationLevel;    // Level of tank deformation

    /// Count of collisions with buildings
    std::map<const Building*, int> collisionCounters;

    // Getter methods
    /// BODY
    glm::vec3 GetBodyPosition() const { return position; }
    float GetBodyRadius() const { return radius; }
    /// WHEEL1
    glm::vec3 GetWheel1Position() const { return position + glm::vec3(1.0f, 0.0f, 0.0f); }
    float GetWheel1Radius() const { return radius * 0.5f; }
    /// WHEEL2
    glm::vec3 GetWheel2Position() const { return position - glm::vec3(1.0f, 0.0f, 0.0f); }
    float GetWheel2Radius() const { return radius * 0.5f; }
};


/// <summary>
/// PLAYER TANK THAT FOLLOW THE ENEMY TANKS AND SHOOT THEM,
/// TRIES TO AVOID COLLISIONS WITH BUILDINGS
/// </summary>
class PlayerTank {
public:
    glm::vec3 position;       // Position of the player tank
    float health;             // Health of the player tank
    float radius;             // Collision radius of the tank

    float turretRotation;     // Rotation angle of the tank's turret
    float tankRotation;       // Rotation angle of the tank's body

    float trajectoryAngle;    // Angle of the tank's trajectory
    float cannonAngle;        // Angle of the tank's cannon

    float deformationLevel;   // Level of tank deformation

    /// <summary>
    /// Initialize a new instance of the PlayerTank class with default values.
    /// </summary>
    PlayerTank() : position(0.0f), health(100.0f), radius(2.2f),
                   turretRotation(0.0f), tankRotation(0.0f), trajectoryAngle(0.0f),
                   cannonAngle(0.0f), deformationLevel(0.0f) {}
};

///
class EnemyTanks {
public:
    /// Collision between an enemy tank and a building
    static bool TankBuildingCollision(
        EnemyTank& enemy,
        Building& building
    );

    /// Collision between two enemy tanks
    static bool TankTankCollision(
        const EnemyTank& enemy1,
        const EnemyTank& enemy2
    );

    /// Collision between an enemy tank and the player tank
    static bool TankPlayerCollision(
        const PlayerTank& player,
        const EnemyTank& enemy
    );

    /// Change the movement pattern of an enemy tank
    static void ChangeMovementPattern(
        EnemyTank& enemy
    );

    /// Enemy tank movement calculated at each moment
    static glm::vec3 EnemyMovement(
        EnemyTank& enemy,
        float deltaTime
    );

    /// Check collision with buildings
    static bool CollisionWithBuildings(
        const glm::vec3& newPosition,
        const std::vector<Building>& buildings
    );

    /// Check collision with tanks
    static bool CollisionWithTanks(
        const glm::vec3& newPosition,
        const std::vector<EnemyTank>& enemies,
        const EnemyTank& currentTank
    );

    /// Fire a projectile from an enemy tank
    static void FireProjectile(
        EnemyTank& enemy,
        std::vector<Projectile>& projectiles,
        const glm::vec3& playerPos
    );

    /// Try to fire at the player, best moment to shoot
    static void TryFireAtPlayer(
        EnemyTank& enemy,
        float deltaTime,
        
        float targetRotation,
        float fireThreshold,

        glm::vec3& playerPos,
        std::vector<Projectile>& projectiles
    );

    /// Rotate towards a player position
    static float RotateTowards(
        float currentRotation,
        float targetRotation,
        float maxRotation
    );

    /// Update sinking enemy tanks
    static void UpdateSinkingTanks(
        std::vector<EnemyTank>& enemies,
        float deltaTime
    );

    /// Update tank collisions enemies and player
    static void UpdateTankCollisions(
        std::vector<EnemyTank>& enemies,
        PlayerTank& player
    );

    /// Update tank collisions with buildings
    static void UpdateTankCollisionsWithBuildings(
        std::vector<EnemyTank>& tanks,
        std::vector<Building>& buildings
    );

    /// Update turret and fire
    static void UpdateTurretAndFire(
        EnemyTank& enemy,
        float deltaTimeSeconds,
        const glm::vec3& playerPos,
        float fireRate,
        std::vector<Projectile>& projectiles);

    /// Update enemy movement
    static void UpdateEnemyMovement(
        std::vector<EnemyTank>& enemies,
        PlayerTank& player,

        bool stopEnemyMovement,
        float deltaTime,

        float attackRange,
        float fireRate,
        float fireAlignmentThreshold,

        float& turretRotationSpeed,
        float& targetRotation,

        std::vector<Projectile>& projectiles,
        const std::vector<Building>& buildings
    );
};

#endif // ENEMY_TANKS_H
