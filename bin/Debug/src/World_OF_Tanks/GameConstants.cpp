#include "GameConstants.h"


//////////////////////////////////////////////////////////////
//                   TANK PLAYER CONSTANTS                  //
//////////////////////////////////////////////////////////////

// Dimensions
const float bodyLength = 2.0f; // Length of the tank body
const float bodyWidth = 1.5f; // Width of the tank body
const float turretWidth = 0.5f; // Width of the tank's turret
const float wheelWidth_PLAYER = 0.3f; // Width of the wheels (PLAYER)
const float wheelDiameter = 0.3f; // Diameter of the wheels
const float bodyHeight = 0.5f; // Height of the tank body
const float turretHeight = 0.4f; // Height of the tank's turret
const float wheelHeight = 0.3f; // Height of the wheels
const float cannonHeight = 1.2f; // Height of the tank's cannon
const float liftHeight = wheelHeight * 0.5f; // Height of the lift for the tank

// Vertical Offsets
const float bodyYOffset = 0.25f; // Vertical offset to raise the body above the ground
const float turretYOffset = bodyHeight; // Vertical offset to position the turret on top of the body
const float cannonYOffset = turretHeight; // Vertical offset to position the cannon on top of the turret
const float wheelsYOffset = wheelHeight / 2; // Vertical offset to sink the wheels into the ground

// Wheel Offsets
const float wheelOffsetFromCenter = (bodyWidth + wheelWidth_PLAYER) / 2; // Offset of the wheels from the center of the tank body
const float wheelYOffset = bodyYOffset - wheelHeight / 2; // Vertical offset to raise the wheels
const float largerBaseWidth = wheelWidth_PLAYER * 1.2f; // Width of the tank's base (20% larger than the top)
const float wheelOffsetWidth = 0.6f; // Horizontal offset for the wheels from the center of the tank

// Colors
const glm::vec3 bodyColor = glm::vec3(0.0f, 0.5f, 0.0f); // DARK GREEN
const glm::vec3 turretColor = glm::vec3(0.2f, 0.7f, 0.2f); // LIGHTER GREEN
const glm::vec3 cannonColor = glm::vec3(0.75f, 0.75f, 0.75f); // GRAY
const glm::vec3 wheelColor = glm::vec3(0.3f, 0.3f, 0.32f); // METALLIC GRAY

//////////////////////////////////////////////////////////////
//                  ENEMY TANK CONSTANTS                    //
//////////////////////////////////////////////////////////////

// Dimensions and Colors
const float wheelWidth_ENEMY = 1.2f; // Width of the wheels (ENEMY)
const glm::vec3 enemyBodyColor = glm::vec3(0.0f, 0.2f, 0.6f); // DARK BLUE
const glm::vec3 enemyTurretColor = glm::vec3(0.1f, 0.4f, 0.8f); // MEDIUM BLUE
const glm::vec3 enemyCannonColor = glm::vec3(0.9f, 0.9f, 0.9f); // LIGHT GRAY
const glm::vec3 enemyWheelColor = glm::vec3(0.5f, 0.5f, 0.5f); // GRAY

//////////////////////////////////////////////////////////////
//              BUILDING AND GAMEPLAY CONSTANTS             //
//////////////////////////////////////////////////////////////

// Building Spacing
const int numTries = 30; // Number of tries for placing buildings
const int randInitBuildings = 10; // Randomly initialize buildings
const float minBuildingSpacing = 5.0f; // Minimum distance between two buildings
const float maxBuildingSpacing = 25.0f; // Maximum distance between buildings
const float maxCubeOffset = 1.5f; // Maximum offset for cube size

// Gameplay Parameters
const float damage = 20.f; // Damage caused by tank projectiles
const float fireRate = 1.0f; // Time between shots (seconds)
const float attackRange = 5.0f; // Range within which enemies will attack
const float fireAlignmentThreshold = 5.0f; // Alignment threshold for turret firing (degrees)
const float rotationThreshold = 5.0f; // Rotation threshold angle to stop rotation
    
extern const int randInitEnemies = 5; // Randomly initialize enemies
const static const int planeSize = 40; // Size of the game plane
const float projectileLifetime = 5.0f; // Lifetime of a projectile (seconds)
