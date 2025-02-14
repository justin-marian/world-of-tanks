#pragma once

#ifndef BUILDINGS_H
#define BUILDINGS_H

#include "Projectiles.h"
#include "EnemyTanks.h"

#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>
#include <string>
#include <vector>


struct Projectile;
struct PlayerTank;


/// <summary>
/// Represent a building in the game world with a position, scale, name, and collision radius.
/// </summary>
struct Building {
    glm::vec3 position; // Position of the building
    glm::vec3 scale;    // Half-extents of the AABB (Axis-Aligned Bounding Box)
    std::string name;   // A unique name for the building
    float radius;       // Collision radius of the building

    // Constructor for easy creation of Building objects
    Building(
        const glm::vec3& pos,const glm::vec3& scl, const std::string& nm, const float radius)
        : position(pos), scale(scl), name(nm), radius(radius) {}

    /// <summary>
    /// Check if a point is inside the building's AABB (Axis-Aligned Bounding Box).
    /// </summary>
    /// <param name="point">The point to check for containment.</param>
    /// <returns>True if the point is inside the building's AABB; otherwise, false.</returns>
    bool containsPoint(const glm::vec3& point) const {
        return  (point.x >= position.x - scale.x && point.x <= position.x + scale.x) &&
                (point.y >= position.y - scale.y && point.y <= position.y + scale.y) &&
                (point.z >= position.z - scale.z && point.z <= position.z + scale.z);
    }
};


/// <summary>
/// Provide functionality for handling collisions with buildings in the game world.
/// </summary>
class Buildings {
public:
    /// Resolve collision between a tank and a building, adjusting the tank's position if needed.
    static void SolveTankBuildingCollision(
        const Building& building,
        PlayerTank& player
    );

    /// Checks for collision between the player's tank and a specific building.
    static bool CheckTankBuildingCollision(
        const Building& building,
        PlayerTank& player
    );

    /// Update collision, check between the player's tank and buildings.
    static void UpdateTankBuildingCollision(
        std::vector<Building>& buildings,
        PlayerTank& player
    );

    /// Update collision, check between projectiles and buildings.
    static void UpdateProjectileBuildingCollisiong(
        std::vector<Building>& buildings,
        std::vector<Projectile>& projectiles,
        float deltaTime
    );
};

#endif // BUILDINGS_H
