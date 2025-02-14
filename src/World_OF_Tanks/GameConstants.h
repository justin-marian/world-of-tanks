#pragma once

#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

#include <glm/glm.hpp>


    // Tank Dimensions
extern const float bodyHeight;
extern const float bodyWidth;
extern const float turretHeight;
extern const float wheelHeight;
extern const float cannonHeight;
extern const float liftHeight;

/// TANK PLAYER
// Body's dimensions to calculate the correct positions for turret and wheels
extern const float bodyLength;
extern const float turretLength;
extern const float turretWidth;

extern const float wheelWidth_PLAYER; // Width of the wheel for the player's tank
extern const float wheelWidth_ENEMY;  // Width of the wheel for enemy tanks
extern const float wheelDiameter;     // Diameter of the wheel

// Tank components vertical adjustments
extern const float bodyYOffset;   // Half the body height to raise it above the ground
extern const float turretYOffset; // Positioned on top of the body
extern const float cannonYOffset; // Positioned on top of the turret
extern const float wheelsYOffset; // Half the wheel's height to sink it into the ground
// Wheel offset from the center of the tank body
extern const float wheelOffsetFromCenter;
// Wheel offset from the center of the tank body
extern const float wheelYOffset; // Adjust this to raise the wheels
/// TANK PLAYER

/// ENEMY TANK
// Define offsets and dimensions for enemy tanks
extern const float wheelOffsetWidth; // Horizontal offset for the wheels from the center of the tank
extern const glm::vec3 bodyColor;    // Dark green for the body
extern const glm::vec3 turretColor;  // Lighter green for the turret and cannon
extern const glm::vec3 cannonColor;  // Gray for the wheels
extern const glm::vec3 wheelColor;   // Metallic gray for the cannon

extern const glm::vec3 enemyBodyColor;   // Dark blue for the body of enemy tanks
extern const glm::vec3 enemyTurretColor; // Medium blue for the turret
extern const glm::vec3 enemyCannonColor; // Light gray for the cannon
extern const glm::vec3 enemyWheelColor;  // Gray for the wheels of enemy tanks
/// ENEMY TANK

// Building Constants
extern const int numTries;
extern const int randInitBuildings;
extern const float minBuildingSpacing; // The minimum distance between any two buildings
extern const float maxBuildingSpacing; // The maximum distance allowed between buildings
extern const float maxCubeOffset;      // Maximum offset for the cube size

// Gameplay Constants
extern const float damage;
extern const float fireRate;                // Seconds between shots
extern const float attackRange;             // Range within which enemies will attack
extern const float fireAlignmentThreshold;  // Degrees within which the turret must be aligned to fire
extern const float rotationThreshold;       // Threshold angle to stop rotation

// Other Constants
extern const int randInitEnemies;
extern const int planeSize;               // Declare planeSize as a static constant
extern const float projectileLifetime;    // Lifetime of a projectile in seconds

#endif // GAME_CONSTANTS_H
