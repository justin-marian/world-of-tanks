#include "GameInit.h"
#include "Renderer.h"
#include "World_OF_Tanks.h"

#include <iostream>
#include <vector>
#include <random>


GameInit::GameInit(World_OF_Tanks* world) : world(world)
{ /* DEFAULT EMPTY CONSTRUCTOR BODY */ }


/// <summary>
/// Generate a random float within a given range.
/// </summary>
/// <param name="min">Minimum value of the range (inclusive).</param>
/// <param name="max">Maximum value of the range (exclusive).</param>
/// <returns>
/// A random float value within the specified range [min, max).
/// </returns>
float randP(float min, float max)
{
    return ((max - min) * ((float)rand() / RAND_MAX)) + min;
}


/// <summary>
/// Check if a given object is overlapping with any buildings in the game world.
/// </summary>
/// <param name="position">Position of the object to check for overlap.</param>
/// <param name="tankRadius">Radius of the object being checked.</param>
/// <returns>
///   <c>true</c> If overlap is detected with any buildings; otherwise, <c>false</c>.
/// </returns>
bool GameInit::IsOverlappingWithBuildings(const glm::vec3& position, float tankRadius)
{
    // Iterate through all buildings to check for overlap.
    for (const Building& building : world->GetBuildings())
    {
        // Distance to the building.
        float distance = glm::distance(position, building.position);
        // If within the sum of radius, overlap is detected.
        if (distance < (tankRadius + building.radius))
        {
            // Overlap detected.
            return true;
        }
    }
    // No overlap.
    return false;
}


/// <summary>
/// Initialize building meshes by creating a certain number of cubes at random positions on the game map.
/// Each building's position is determined so that it does not overlap with any existing buildings.
/// Buildings are spaced out according to specified minimum and maximum values.
/// </summary>
void GameInit::InitializeBuildings()
{
    // Generate a random number of cubes to represent buildings
    // Randomly choose a number between 10 and 20 for the number of buildings
    int numBuildings = rand() % randInitBuildings + randInitBuildings;
    world->SetNumBuildings(numBuildings);

    // Determine the grid size based on the plane size and minimum building spacing
    int numPositionsX = static_cast<int>((planeSize - 2 * maxCubeOffset) / minBuildingSpacing);
    int numPositionsZ = static_cast<int>((planeSize - 2 * maxCubeOffset) / minBuildingSpacing);

    // Track which positions on the grid are already occupied
    std::vector<std::pair<int, int>> occupiedPositions;

    // Create each building
    for (int i = 0; i < numBuildings; ++i)
    {
        bool validPositionFound = false;    // Check if a valid position is found
        glm::vec3 position;                 // Position of the building
        glm::vec3 scale;                    // Size of the building

        // Attempt to find a valid position up to 30 times
        int tries = 0;

        while (!validPositionFound && tries < numTries)
        {
            // Randomly scale the building
            scale = glm::vec3(randP(0.5f, maxCubeOffset), 2 * randP(0.5f, maxCubeOffset), randP(0.5f, maxCubeOffset));
            
            // Choose a random position on the grid
            int gridX = rand() % numPositionsX;
            int gridZ = rand() % numPositionsZ;

            // Buildings are centered in the world, battle space!
            float buildingsLimits = static_cast<float>(planeSize / 2);

            // Convert grid position to world coordinates
            float worldX = (gridX * minBuildingSpacing) + minBuildingSpacing / 2 - buildingsLimits + maxCubeOffset;
            float worldZ = (gridZ * minBuildingSpacing) + minBuildingSpacing / 2 - buildingsLimits + maxCubeOffset;
            position = glm::vec3(worldX, scale.y, worldZ);

            // Check if the selected position overlaps with any existing buildings
            validPositionFound = true;

            for (const auto& pos : occupiedPositions)
            {
                if (std::abs(pos.first - gridX) * minBuildingSpacing < minBuildingSpacing &&
                    std::abs(pos.second - gridZ) * minBuildingSpacing < minBuildingSpacing)
                {
                    validPositionFound = false;
                    break;
                }
            }

            // If a valid position is found, save it
            if (validPositionFound)
            {
                occupiedPositions.push_back(std::make_pair(gridX, gridZ));
            }

            tries++;
        }

        // Valid position was found, create the mesh for the building
        if (validPositionFound)
        {
            // Create vertices for the cube
            std::vector<VertexFormat> vertices
            {
                VertexFormat(position + glm::vec3(-1, -1,  1) * scale,
                glm::vec3(randP(0.0f, 1.0f), randP(0.0f, 1.0f), randP(0.0f, 1.0f))),
                VertexFormat(position + glm::vec3(1, -1,  1) * scale,
                glm::vec3(randP(0.0f, 1.0f), randP(0.0f, 1.0f), randP(0.0f, 1.0f))),
                VertexFormat(position + glm::vec3(-1,  1,  1) * scale,
                glm::vec3(randP(0.0f, 1.0f), randP(0.0f, 1.0f), randP(0.0f, 1.0f))),
                VertexFormat(position + glm::vec3(1,  1,  1) * scale,
                glm::vec3(randP(0.0f, 1.0f), randP(0.0f, 1.0f), randP(0.0f, 1.0f))),
                VertexFormat(position + glm::vec3(-1, -1, -1) * scale,
                glm::vec3(randP(0.0f, 1.0f), randP(0.0f, 1.0f), randP(0.0f, 1.0f))),
                VertexFormat(position + glm::vec3(1, -1, -1) * scale,
                glm::vec3((0.0f, 1.0f), randP(0.0f, 1.0f), randP(0.0f, 1.0f))),
                VertexFormat(position + glm::vec3(-1,  1, -1) * scale,
                glm::vec3(randP(0.0f, 1.0f), randP(0.0f, 1.0f), randP(0.0f, 1.0f))),
                VertexFormat(position + glm::vec3(1,  1, -1) * scale,
                glm::vec3(randP(0.0f, 1.0f), randP(0.0f, 1.0f), randP(0.0f, 1.0f))),
            };

            // Create indices for the cube
            std::vector<unsigned int> indices =
            {
                0, 1, 2,  1, 3, 2,
                2, 3, 7,  2, 7, 6,
                1, 7, 3,  1, 5, 7,
                6, 7, 4,  7, 5, 4,
                0, 4, 1,  1, 4, 5,
                2, 6, 4,  0, 2, 4,
            };

            // Calculate a radius for collision detection
            float r = 0.5f * sqrt(scale.x * scale.x + scale.y * scale.y + scale.z * scale.z);

            std::string name = "cube" + std::to_string(i);
            // Log the creation of a building
            std::cout << "Created BUILDING: " << name << std::endl;

            // Add the building to the list of buildings
            world->AddBuilding(Building(position, scale, name, r));
            // Use the CreateMesh function to create and store the mesh
            world->renderer->CreateMesh(name.c_str(), vertices, indices);
        }
    }
}


/// <summary>
/// Initialize enemy tanks by creating a specified number of tanks with random attributes.
/// Tank is placed in a position that does not overlap with any buildings.
/// </summary>
void GameInit::InitializeEnemyTanks()
{
    // The number of enemy tanks to generate
    int numEnemies = rand() % randInitEnemies + randInitEnemies;

    // Create each enemy tank
    for (int i = 0; i < numEnemies; ++i)
    {
        EnemyTank enemy;
        // Flag for checking position validity.
        bool validPositionFound = false;

        // Attempt to find a valid position up to 30 times.
        int tries = 0;

        // Try to find a non-overlapping position for each tank.
        while (!validPositionFound && tries < 30)
        {
            // Randomize position and check for overlap with buildings
            enemy.position = glm::vec3(randP(-20.0f, 20.0f), 0.0f, randP(-20.0f, 20.0f));
            enemy.radius = 1.5f; // Set the collision radius

            // Check for overlap with buildings
            if (!IsOverlappingWithBuildings(enemy.position, enemy.radius))
            {
                validPositionFound = true; // Valid Position
            }

            tries++;
        }

        // Position was found, set the tank's attributes
        if (validPositionFound)
        {
            // Set initial attributes
            enemy.health = 100;                             // Full health
            ///
            enemy.rotation = randP(0.0f, 360.0f);           // Random rotation in degrees
            enemy.turretRotation = randP(0.0f, 360.0f);     // Random turret rotation in degrees
            ///
            enemy.isPlayerInRange = false;                  // Initial state
            enemy.movementTimer = randP(3.0f, 10.0f);       // Random timer for changing movement pattern
            enemy.timeSinceLastShot = 0.0f;                 // Reset shot timer

            world->AddEnemy(enemy);
        }
        else
        {
            std::cout << "ERROR: NO OVERLAPING POSITIONS TANKS " 
                      << i << " could not be found." << std::endl;
        }
    }
}
