#include "Buildings.h"

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

#include <glm/glm.hpp>


/// <summary>
/// Check for collision between a player-controlled tank and a building.
/// </summary>
/// <param name="building">Building to check for collision with.</param>
/// <param name="player">Player-controlled tank.</param>
/// <returns>True if collision is detected; otherwise, false.</returns>
bool Buildings::CheckTankBuildingCollision(
    const Building& building,
    PlayerTank& player)
{
    // Combined radius of the tank and building
    float combinedRadius = player.radius + building.radius;
    // Squared distance between the tank and the building
    float distanceSquared = glm::distance2(player.position, building.position);
    // Check if the squared distance is less than the squared combined radius
    return distanceSquared < (combinedRadius * combinedRadius);
}


/// <summary>
/// Solve collision between a player-controlled tank and a building with penetration correction.
/// </summary>
/// <param name="building">Building to check for collision with.</param>
/// <param name="player">Player-controlled tank.</param>
/// <returns>True if collision is detected; otherwise, false.</returns>
void Buildings::SolveTankBuildingCollision(
    const Building& building,
    PlayerTank& player)
{
    // The orientation of the vehicle when collision happend!
    glm::vec3 collisionDir = glm::normalize(player.position - building.position);
    // Squared distance between the tank and the building
    float distanceSquared = glm::distance2(player.position, building.position);
    // Combined radius of the tank and building
    float combinedRadius = player.radius + building.radius;
    float penetrationDepth = combinedRadius - glm::sqrt(distanceSquared);
    // Adjust the displacement vector
    glm::vec3 displacement = penetrationDepth * collisionDir;
    displacement.y = 0; // FLAT PLANE
    // Adjust the tank's position
    player.position += displacement;
}


/// <summary>
/// Update collision, check between a player-controlled tank and a vector of buildings.
/// </summary>
/// <param name="buildings">Vector of buildings in the game world.</param>
/// <param name="player">Player-controlled tank.</param>
void Buildings::UpdateTankBuildingCollision(
    std::vector<Building>& buildings,
    PlayerTank& player)
{
    for (const Building& building : buildings)
    {
        if (CheckTankBuildingCollision(building, player))
        {
            // Resolve collision with the building
            SolveTankBuildingCollision(building, player);
        }
    }
}


/// <summary>
/// Update collision, check between projectiles and a vector of buildings.
/// </summary>
/// <param name="buildings">Vector of buildings in the game world.</param>
/// <param name="projectiles">Veector of projectiles in the game world.</param>
/// <param name="deltaTime">Time elapsed since the last update.</param>
void Buildings::UpdateProjectileBuildingCollisiong(
    std::vector<Building>& buildings,
    std::vector<Projectile>& projectiles,
    float deltaTime)
{
    for (auto projectileIt = projectiles.begin(); projectileIt != projectiles.end(); )
    {
        // Update the projectile's position based on its velocity and time
        projectileIt->position += projectileIt->velocity * deltaTime;
        bool collision = false;

        // Check for collision with buildings
        for (const auto& building : buildings)
        {
            if (Projectiles::ProjectileBuildingCollision(*projectileIt, building))
            {
                // Remove the projectile when a collision is detected
                projectileIt = projectiles.erase(projectileIt);
                collision = true;
                break; // DON'T NEED TO CHECK OTHER BUILDINGS
            }
        }

        if (!collision)
        {
            // No collision detected, move to the next projectile
            ++projectileIt;
        }
    }
}
