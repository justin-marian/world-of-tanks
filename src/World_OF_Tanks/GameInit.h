#pragma once

#ifndef GAMEINIT_H
#define GAMEINIT_H

#include <vector>
#include <glm/glm.hpp>


class World_OF_Tanks;

// GameInit class is responsible for initializing environment game elements //
//                      TANK ENEMIES AND BUILDINGS                          //
class GameInit {
public:
    // GameInit interactions with the buildings and enemies from the game world.
    GameInit(World_OF_Tanks* world);

    /// Initialize building meshes in the game world.
    // Buildings are created and placed without overlapping.
    void InitializeBuildings();
    // EnemyTanks are placed in valid positions without overlapping with buildings.
    void InitializeEnemyTanks();

private:
    // Access & Modify the game world, adding buildings and enemies.
    World_OF_Tanks* world;
        
    /// Check if a given position overlaps with any existing buildings.
    // New TankEnemis don't overlap with buildings.
    bool IsOverlappingWithBuildings(const glm::vec3& position, float tankRadius);
};

#endif // GAMEINIT_H
