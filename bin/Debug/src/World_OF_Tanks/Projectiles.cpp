#include "Projectiles.h"
#include "Buildings.h"

#include <glm/gtc/constants.hpp>


/// <summary>
/// Check for collision between a projectile and an enemy tank.
/// </summary>
/// <param name="projectile">Projectile to check.</param>
/// <param name="tank">Enemy tank to check against.</param>
/// <returns>True if a collision is detected, otherwise false.</returns>
bool Projectiles::ProjectileTankCollision(
    const Projectile& projectile,
    const EnemyTank& tank)
{
    // Distance between the projectile and tank centers
    glm::vec3 dif = tank.position - projectile.position;
    float distance = glm::length(dif);

    // Check if a collision occurred
    if (distance < 1.f)
    {
        return true; // Collision
    }
    return false; // No collision
}


/// <summary>
/// Check for collision between a projectile and the player's tank.
/// </summary>
/// <param name="projectile">Projectile to check.</param>
/// <param name="player">Player's tank to check against.</param>
/// <returns>True if a collision is detected, otherwise false.</returns>
bool Projectiles::ProjectilePlayerCollision(
    const Projectile& projectile,
    const PlayerTank& player)
{
    // Distance between the projectile and tank centers
    glm::vec3 dif = player.position - projectile.position;
    float distance = glm::length(dif);

    // Check if a collision occurred
    if (distance < 1.f)
    {
        return true; // Collision
    }
    return false; // No collision
}


/// <summary>
/// Check for collision between a projectile and a building.
/// </summary>
/// <param name="projectile">Projectile to check.</param>
/// <param name="building">Building to check against.</param>
/// <returns>True if a collision is detected, otherwise false.</returns>
bool Projectiles::ProjectileBuildingCollision(
    const Projectile& projectile,
    const Building& building)
{
    glm::vec3 projectileMinBox = projectile.position - glm::vec3(projectile.radius);
    glm::vec3 projectileMaxBox = projectile.position + glm::vec3(projectile.radius);

    glm::vec3 buildingMinBox = building.position - building.scale * 0.5f;
    glm::vec3 buildingMaxBox = building.position + building.scale * 0.5f;

    // Check for overlap in each dimension
    bool collisionX = projectileMinBox.x <= buildingMaxBox.x && projectileMaxBox.x >= buildingMinBox.x;
    bool collisionY = projectileMinBox.y <= buildingMaxBox.y && projectileMaxBox.y >= buildingMinBox.y;
    bool collisionZ = projectileMinBox.z <= buildingMaxBox.z && projectileMaxBox.z >= buildingMinBox.z;

    // Collision only if overlap occurs in all dimensions
    return collisionX && collisionY && collisionZ;
}



/// <summary>
/// Update projectiles to check for collisions with the player's tank and apply damage.
/// </summary>
/// <param name="projectiles">Vector of projectiles to update.</param>
/// <param name="player">Player's tank to check for collisions.</param>
/// <param name="damage">Damage to apply to the player's tank upon collision.</param>
void Projectiles::UpdateProjectilesPlayerCollision(
    std::vector<Projectile>& projectiles,
    PlayerTank& player,
    int damage)
{
    for (auto itProjectile = projectiles.begin(); itProjectile != projectiles.end(); )
    {
        // Check for collision with the player's tank
        if (Projectiles::ProjectilePlayerCollision(*itProjectile, player))
        {
            // Reduce player's health
            player.health -= damage;
            // Projectile dissapear
            itProjectile = projectiles.erase(itProjectile);
        }
        else
        {
            // Next projectile
            ++itProjectile;
        }
    }
}


/// <summary>
/// Update projectile movements and check for collisions with enemy tanks.
/// </summary>
/// <param name="projectiles">Vector of projectiles to update.</param>
/// <param name="enemies">Vector of enemy tanks to check for collisions.</param>
/// <param name="projectileDamage">Damage to apply to enemy tanks upon collision.</param>
/// <param name="deltaTime">Time elapsed since the last update.</param>
void Projectiles::UpdateProjectileMovementsAndCollisions(
    std::vector<Projectile>& projectiles,
    std::vector<EnemyTank>& enemies,
    int projectileDamage,
    float deltaTime)
{
    for (auto itProjectile = projectiles.begin(); itProjectile != projectiles.end(); )
    {
        itProjectile->position += itProjectile->velocity * deltaTime;
        itProjectile->lifespan += deltaTime;

        if (itProjectile->lifespan >= itProjectile->maxLifespan)
        {
            itProjectile = projectiles.erase(itProjectile); // Erase due to lifespan expiry
        }
        else
        {
            bool collisionDetected = false;
            for (auto& enemy : enemies) {
                if (!enemy.isDestroyed && ProjectileTankCollision(*itProjectile, enemy)) {

                    enemy.health -= projectileDamage;
                    enemy.deformationLevel = std::min(enemy.deformationLevel + 0.1f, 1.0f);

                    if (enemy.health <= 0) {
                        enemy.isDestroyed = true;
                    }

                   collisionDetected = true;
                    break; // Exit enemy loop
                }
            }

            if (collisionDetected)
            {
                // Erase due to collision
                itProjectile = projectiles.erase(itProjectile);
            }
            else
            {
                // Move to next projectile
                ++itProjectile;
            }
        }
    }
}
