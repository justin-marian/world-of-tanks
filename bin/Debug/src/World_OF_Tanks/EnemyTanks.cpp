#include "EnemyTanks.h"
#include "Buildings.h"
#include "Projectiles.h"

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

#include <glm/gtc/constants.hpp>
#include <iostream>


#define MIN(a, b) ((a) < (b) ? (a) : (b))


/// <summary>
/// Check for collision between two enemy tanks.
/// </summary>
/// <param name="tank1">The first enemy tank.</param>
/// <param name="tank2">The second enemy tank.</param>
/// <returns>True if collision is detected; otherwise, false.</returns>
bool EnemyTanks::TankTankCollision(
    const EnemyTank& tank1,
    const EnemyTank& tank2) 
{
    // Calculate the distance between the two tank positions
    return glm::distance(tank1.position, tank2.position) < 
                        (tank1.radius + tank2.radius);
}


/// <summary>
/// Check for collision between a player-controlled tank and an enemy tank.
/// </summary>
/// <param name="player">The player-controlled tank.</param>
/// <param name="enemyTank">The enemy tank to check for collision with.</param>
/// <returns>True if collision is detected; otherwise, false.</returns>
bool EnemyTanks::TankPlayerCollision(
    const PlayerTank& player,
    const EnemyTank& enemyTank)
{
    // Calculate the distance between the player tank and the enemy tank positions
    return glm::distance(player.position, enemyTank.position) < 
                        (player.radius + enemyTank.radius);
}


/// <summary>
/// Check for collision between an enemy tank and a building, and resolve it if detected.
/// <param name="tank">The enemy tank to check for collision.</param>
/// <param name="building">The building to check for collision with.</param>
/// <returns>True if collision is detected and resolved; otherwise, false.</returns>
bool EnemyTanks::TankBuildingCollision(
    EnemyTank& tank,
    Building& building)
{
    // Calculate the combined radius of the tank and building
    float combinedRadius = tank.radius + 1.1f + building.radius;
    // Calculate the squared distance between the tank and building
    float distanceSqr = glm::distance2(tank.position, building.position);

    // Check if the squared distance is less than the squared combined radius
    if (distanceSqr < (combinedRadius * combinedRadius))
    {
        // Calculate the direction from the tank to the building
        glm::vec3 collisionDir = glm::normalize(tank.position - building.position);
        float penetrationDepth = combinedRadius - glm::sqrt(distanceSqr);

        // Calculate the displacement vector to resolve the collision
        // Move the tank away from the building to resolve the collision
        glm::vec3 displacement = penetrationDepth * collisionDir;
        displacement.y = 0;
        tank.position += displacement;

        // Return true to indicate a collision
        return true;
    }

    // No collision detected
    return false;
}


/// <summary>
/// Update sinking depth and renderability of destroyed enemy tanks.
/// </summary>
/// <param name="enemies">Vector of enemy tanks.</param>
/// <param name="deltaTime">Time elapsed since the last update.</param>
void EnemyTanks::UpdateSinkingTanks(
    std::vector<EnemyTank>& enemies,
    float deltaTime)
{
    for (auto& tank : enemies)
    {
        if (tank.isDestroyed)
        {
            // Update sinking depth
            tank.sinkDepth += deltaTime * tank.sinkSpeed;
            // Make tank non-renderable once it has sunk ENOUGH
            if (tank.sinkDepth >= 1.f)
            {
                tank.isRenderable = false;
            }
        }
    }
}


/// <summary>
/// Calculate the displacement vector needed to resolve collision between two objects.
/// </summary>
/// <param name="pos1">Position of the first object.</param>
/// <param name="radius1">Radius of the first object.</param>
/// <param name="pos2">Position of the second object.</param>
/// <param name="radius2">Radius of the second object.</param>
/// <returns>Displacement vector to resolve the collision.</returns>
glm::vec3 CalculateCollisionDisplacement(
    const glm::vec3& pos1, float radius1,
    const glm::vec3& pos2, float radius2)
{
    // Vector between the positions of the two objects
    glm::vec3 diff = pos1 - pos2;
    float distance = glm::length(diff);
    float overlap = (radius1 + radius2) - distance;

    // Displacement vector SOLVE the collision
    if (overlap > 0)
    {
        return glm::normalize(diff) * overlap;
    }
    return glm::vec3(0.0f);
}


/// <summary>
/// Update tank collisions with the player and other enemy tanks and apply smoothing to positions.
/// </summary>
/// <param name="enemies">Vector of enemy tanks.</param>
/// <param name="player">Player tank.</param>
void EnemyTanks::UpdateTankCollisions(
    std::vector<EnemyTank>& enemies,
    PlayerTank& player)
{
    float smoothingFactor = 0.1f;

    for (auto& enemy1 : enemies)
    {
        // Store the previous position before handling collisions
        glm::vec3 prevPos = enemy1.position;

        // Handle player and enemy tank collisions
        if (TankPlayerCollision(player, enemy1))
        {
            glm::vec3 diff = enemy1.position - player.position;
            float distance = glm::length(diff);
            float overlap = (player.radius + enemy1.radius) - distance;

            if (overlap > 0)
            {
                glm::vec3 displacement = glm::normalize(diff) * (overlap * 0.5f);
                player.position -= displacement;
                enemy1.position += displacement;
            }
        }

        // Handle collisions among enemy tanks
        for (auto& enemy2 : enemies)
        {
            if (&enemy1 != &enemy2 && TankTankCollision(enemy1, enemy2))
            {
                glm::vec3 dif = enemy2.position - enemy1.position;
                float distance = glm::length(dif);
                float overlap = (enemy1.radius + enemy2.radius) - distance;

                if (overlap > 0)
                {
                    glm::vec3 displacement = glm::normalize(dif) * (overlap * 0.5f);
                    enemy1.position -= displacement;
                    enemy2.position += displacement;
                }
            }
        }

        // Calculate the smooth displacement
        glm::vec3 smoothDisplace = enemy1.position - prevPos;

        // Apply smoothing to the positions
        enemy1.position -= smoothDisplace * smoothingFactor;
        player.position += smoothDisplace * smoothingFactor;
    }
}


/// <summary>
/// Update tank collisions with buildings and apply smoothing to positions.
/// </summary>
/// <param name="tanks">Vector of enemy tanks.</param>
/// <param name="buildings">Vector of buildings.</param>
void EnemyTanks::UpdateTankCollisionsWithBuildings(
    std::vector<EnemyTank>& tanks,
    std::vector<Building>& buildings)
{
    float smoothingFactor = 0.1f;

    for (EnemyTank& tank : tanks)
    {
        // Store the previous position before handling collisions
        glm::vec3 previousPosition = tank.position;
        // Total displacement due to collisions with buildings
        glm::vec3 totalDisplacement(0.0f);

        for (Building& building : buildings)
        {
            if (TankBuildingCollision(tank, building))
            {
                glm::vec3 diff = tank.position - building.position;
                float distance = glm::length(diff);
                float overlap = (tank.radius + building.radius) - distance;

                if (overlap > 0)
                {
                    glm::vec3 displacement = glm::normalize(diff) * (overlap * 0.5f);
                    tank.position -= displacement;
                    totalDisplacement += displacement;
                }
            }
        }

        // Apply the total displacement to the tank's position
        tank.position += totalDisplacement;
        glm::vec3 smoothDisplacement = tank.position - previousPosition;
        // Apply smoothing to the positions
        tank.position -= smoothDisplacement * smoothingFactor;
    }
}


/// <summary>
/// Fire a projectile from an enemy tank if the turret is aligned with the player.
/// </summary>
/// <param name="enemy">Enemy tank firing the projectile.</param>
/// <param name="projectiles">Vector containing the projectiles in the game world.</param>
/// <param name="playerPos">Position of the player-controlled tank.</param>
void EnemyTanks::FireProjectile(
    EnemyTank& enemy,
    std::vector<Projectile>& projectiles,
    const glm::vec3& playerPos)
{
    float turretLength = 1.0f;

    // X and Z coords for turret direction based on the turret rotation
    float turretDirectionX = cos(enemy.turretRotation);
    float turretDirectionZ = sin(enemy.turretRotation);

    // Create a normalized turret direction vector in the XZ plane (ignoring the Y-axis)
    glm::vec3 turretDirection = glm::vec3(turretDirectionX, 0.0f, turretDirectionZ);
    // Calculate the direction from the enemy tank to the player tank and normalize it
    glm::vec3 directionToPlayer = glm::normalize(playerPos - enemy.position);

    // Firing logic - only fire if the turret is aligned with the player
    // within a 10-degree firing OPEN RANGE CONE
    if (glm::dot(turretDirection, directionToPlayer) > cos(glm::radians(10.0f)))
    {
        // Adjust the turret tip position based on the size of your tank model
        glm::vec3 turretTipPosition = enemy.position + turretDirection * turretLength;

        // Initial position for the projectile at the tip of the turret
        glm::vec3 worldCannonTip = turretTipPosition;
        worldCannonTip.y += 0.9f;

        // Velocity direction as the forward direction towards the player
        glm::vec3 velocityDirection = glm::normalize(directionToPlayer);

        /// Set the projectile's position and velocity
        Projectile newProjectile;

        newProjectile.position = worldCannonTip;            // START PROJECTILE POSITION
        newProjectile.velocity = velocityDirection * 2.5f;  // SPEED PROJECTILE

        newProjectile.lifespan = 0.0f;                      // START PROJECTILE FIRE
        newProjectile.maxLifespan = 10.0f;                  // TIME SPAN UNTIL HE DISSAPEAR
        newProjectile.radius = 0.10f;                       // RADIUS OF THE PROJECTILE

        projectiles.push_back(newProjectile);
    }
}


/// <summary>
/// Attempt to fire a projectile at the player if the turret alignment threshold is met.
/// </summary>
/// <param name="enemy">Enemy tank attempting to fire.</param>
/// <param name="deltaTime">Elapsed since the last frame.</param>
/// <param name="targetRotation">Desired rotation of the turret.</param>
/// <param name="fireAlignmentThreshold">Threshold for turret alignment before firing.</param>
/// <param name="playerPos">Player's position.</param>
/// <param name="projectiles">Vector of projectiles.</param>
void EnemyTanks::TryFireAtPlayer(
    EnemyTank& enemy,
    float deltaTime,

    float targetRotation,
    float fireAlignmentThreshold,
    
    glm::vec3& playerPos,
    std::vector<Projectile>& projectiles)
{
    // ABS difference in rotation between the enemy's turret and the target rotation.
    float rotationDiff = glm::abs(targetRotation - enemy.turretRotation);

    // Normalize the rotation
    if (rotationDiff > 180.0f) rotationDiff -= 360.0f;

    // ABS rotation difference is within the specified alignment threshold.
    if (glm::abs(rotationDiff) <= fireAlignmentThreshold) FireProjectile(enemy, projectiles, playerPos);
}


/// <summary>
/// Utility function to smoothly rotate the ENEMY turret.
/// <summary>
/// <param name="currentRotation">Current turret rotation.</param>
/// <param name="targetRotation">Target turret rotation.</param>
/// <param name="maxRotation">Maximum turret rotation speed.</param>
/// <returns>New turret rotation.</returns>
float EnemyTanks::RotateTowards(
    float currentRotation,
    float targetRotation,
    float maxRotation)
{
    // Rotation difference to rotate the turret-cannon in the direction of the player TARGET
    float rotationDiff = fmod(targetRotation - currentRotation + M_PI, 2 * M_PI) - M_PI;
    // Rotation speed clamping
    return fmod(currentRotation + std::min(rotationDiff, maxRotation), 2 * M_PI);
}


/// <summary>
/// Update the turret rotation of an enemy tank and fire projectiles if conditions are met.
/// </summary>
/// <param name="enemy">Enemy tank whose turret is to be updated.</param>
/// <param name="deltaTime">Time elapsed since the last update.</param>
/// <param name="playerPos">Position of the player-controlled tank.</param>
/// <param name="fireRate">Rate at which the enemy tank can fire projectiles.</param>
/// <param name="projectiles">Vector containing the projectiles fired by enemy tanks.</param>
void EnemyTanks::UpdateTurretAndFire(
    EnemyTank& enemy,
    float deltaTime,
    const glm::vec3& playerPos,
    float fireRate,
    std::vector<Projectile>& projectiles)
{
    // Enemy tank is still alive
    if (enemy.health > 0)
    {
        // Angle towards the player tank
        glm::vec3 directionToPlayer = playerPos - enemy.position;
        float desiredTurretRotation = M_PI - atan2(directionToPlayer.z, directionToPlayer.x);

        // Smoothly adjust the turret rotation
        enemy.turretRotation = RotateTowards(enemy.turretRotation, desiredTurretRotation, deltaTime * 2.0f);

        // Fire at the player if the turret is aligned with the player
        // and the fire rate cooldown has passed
        if (enemy.timeSinceLastShot >= fireRate)
        {
            FireProjectile(enemy, projectiles, playerPos);
            // Reset the fire delay timer
            enemy.timeSinceLastShot = 0;
        }
        else
        {
            // Time pass, can't shoot for now!
            enemy.timeSinceLastShot += deltaTime;
        }
    }
}


/// <summary>
/// Randomly change the movement pattern of an enemy tank.
/// </summary>
/// <param name="enemy">Enemy tank to change the pattern for.</param>
void EnemyTanks::ChangeMovementPattern(EnemyTank& enemy)
{
    // Randomly change the movement pattern
    // Assuming 4 different patterns (0-3)
    enemy.movementPattern = std::rand() % 4;
}


/// <summary>
/// Handle the movement logic for an enemy tank based on its current movement pattern.
/// </summary>
/// <param name="enemy">Enemy tank to move.</param>
/// <param name="deltaTime">Time since the last frame.</param>
/// <returns>Movement vector for the enemy tank.</returns>
glm::vec3 EnemyTanks::EnemyMovement(
    EnemyTank& enemy,
    float deltaTime)
{
    glm::vec3 movement(0);

    switch (enemy.movementPattern)
    {
    case 0: // Move forward
        movement += glm::vec3(
            cos(enemy.rotation + M_PI) / 20,
            0,
            -sin(enemy.rotation + M_PI) / 20);
        break;

    case 1: // Move backward
        movement += glm::vec3(
            -cos(enemy.rotation + M_PI) / 20,
            0,
            sin(enemy.rotation + M_PI) / 20);
        break;

    case 2: // Rotate clockwise
        enemy.rotation += deltaTime; // Rotate tank body
        break;

    case 3: // Rotate counterclockwise
        enemy.rotation -= deltaTime; // Rotate tank body
        break;
    }

    // Apply boundary clamping
    glm::vec3 newPosition = enemy.position + movement;
    // Clamping newPosition within the map boundaries
    newPosition.x = std::max(-20.0f, std::min(20.0f, newPosition.x));
    newPosition.z = std::max(-20.0f, std::min(20.0f, newPosition.z));
    return newPosition - enemy.position;
}


/// <summary>
/// Check for collisions between an enemy tank and buildings.
/// </summary>
/// <param name="newPosition">New position of the enemy tank.</param>
/// <param name="buildings">Vector of buildings in the game.</param>
/// <returns>True if a collision with buildings is detected, false otherwise.</returns>
bool EnemyTanks::CollisionWithBuildings(
    const glm::vec3& newPosition,
    const std::vector<Building>& buildings) 
{
    // Adjust this based on your game's scale
    const float collisionRadius = 1.0f;

    for (const auto& building : buildings)
    {
        if (glm::distance(newPosition, building.position) < collisionRadius + building.radius)
        {
            return true; // Collision detected
        }
    }
    return false; // No collision
}


/// <summary>
/// Check for collisions between an enemy tank and other enemy tanks.
/// </summary>
/// <param name="newPosition">New position of the enemy tank.</param>
/// <param name="enemies">Vector of enemy tanks in the game.</param>
/// <param name="currentTank">Current enemy tank being checked.</param>
/// <returns>True if a collision with other tanks is detected, false otherwise.</returns>
bool EnemyTanks::CollisionWithTanks(
    const glm::vec3& newPosition,
    const std::vector<EnemyTank>& enemies,
    const EnemyTank& currentTank)
{
    const float collisionRadius = 2.0f; // Adjust this based on your game's scale and tank size

    for (const auto& enemy : enemies)
    {
        if (&enemy != &currentTank && 
            glm::distance(newPosition, enemy.position) < collisionRadius + currentTank.radius)
        {
            return true; // Collision detected with another enemy tank
        }
    }

    return false; // No collision with other tanks
}


/// <summary>
/// Update the movement, turret rotation, and firing of enemy tanks.
/// </summary>
/// <param name="enemies">Vector of enemy tanks.</param>
/// <param name="player">Player's tank.</param>
/// <param name="stopEnemyMovement">Stop enemy movement.</param>
/// <param name="deltaTime">Time since the last frame.</param>
/// <param name="attackRange">Attack range of enemy tanks.</param>
/// <param name="fireRate">Firing rate of enemy tanks.</param>
/// <param name="fireAlignmentThreshold">Alignment threshold for firing at the player.</param>
/// <param name="turretRotationSpeed">Turret rotation speed of enemy tanks.</param>
/// <param name="targetRotation">Target rotation of enemy tanks.</param>
/// <param name="projectiles">Vector of projectiles.</param>
/// <param name="buildings">Vector of buildings.</param>
void EnemyTanks::UpdateEnemyMovement(
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
    const std::vector<Building>& buildings)
{
    for (auto& enemy : enemies)
    {
        // Randomize movement pattern periodically
        if (enemy.movementTimer <= 0) {
            enemy.movementPattern = std::rand() % 4;
            // Change pattern every 1-5 seconds
            enemy.movementTimer = (std::rand() % 5) + 1;
        }
        else
        {
            enemy.movementTimer -= deltaTime;
        }

        if (!stopEnemyMovement)
        {
            glm::vec3 movement = EnemyMovement(enemy, deltaTime);
            glm::vec3 newPosition = enemy.position + movement;

            // Check for collisions with buildings and other enemy tanks
            if (!CollisionWithBuildings(newPosition, buildings) &&
                !CollisionWithTanks(newPosition, enemies, enemy))
            {
                enemy.position = newPosition;
            }
            else
            {
                ChangeMovementPattern(enemy);
            }
        }

        // Turret and firing updates
        UpdateTurretAndFire(enemy, deltaTime, player.position, fireRate, projectiles);

        if (enemy.isPlayerInRange && player.health > 0)
        {
            TryFireAtPlayer(enemy, deltaTime, targetRotation,
                   fireAlignmentThreshold, player.position, projectiles);
        }

        enemy.timeSinceLastShot += deltaTime;
    }
}

