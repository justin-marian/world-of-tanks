#pragma once

#ifndef WORLD_OF_TANKS_H
#define WORLD_OF_TANKS_H

#include "components/simple_scene.h"

#include "Camera3rdPerson.h"
#include "TankComponent.h"

#include "Renderer.h"
#include "GameInit.h"

#include "Buildings.h"
#include "Projectiles.h"
#include "EnemyTanks.h"

#include <map>
#include <random>
#include <vector>
#include <functional>
#include <unordered_map>
#include <unordered_set>


class GameInit;

class World_OF_Tanks : public gfxc::SimpleScene
{
    const float Z_NEAR = 0.1f;
    const float Z_FAR = 100.0f;
public:
    TankComponent::TankComponent tankComponent;
    Renderer* renderer;

    struct ViewportArea
    {
        int x, y;
        int width, height;
        ViewportArea() :
                        x(0), y(0), width(1), height(1) {}
        ViewportArea(int x, int y, int width, int height) :
                        x(x), y(y), width(width), height(height) {}
    };
        
    World_OF_Tanks();
    ~World_OF_Tanks();
    void Init() override;

    void AddBuilding(const Building& building) { buildings.push_back(building); }
    void AddEnemy(const EnemyTank& enemy) { enemies.push_back(enemy); }

    int GetNumBuildings() const { return numBuildings; }
    void SetNumBuildings(int newNumBuildings) { numBuildings = newNumBuildings; }
    const std::vector<Building>& GetBuildings() const { return buildings; }

private:
    void RenderScene(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

    void FrameStart() override;
    void FrameEnd() override;

    void Update(float deltaTimeSeconds) override;

    void OnInputUpdate(float deltaTime, int mods) override;
    void OnKeyPress(int key, int mods) override;
    void OnKeyRelease(int key, int mods) override;
    void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
    void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
    void OnWindowResize(int width, int height) override;

protected:
    GLenum polygonMode;
    glm::ivec2 resolution;
    Camera3rdPerson::Camera camera;

    glm::mat4 projectionMatrix;
    glm::mat4 modelMatrix;

    GameInit gameInit; // Game initialization
    PlayerTank player; // Player's tank

    std::vector<Building> buildings;        // buildings in the scene
    std::vector<Projectile> projectiles;    // projectiles
    std::vector<EnemyTank> enemies;         // enemy tanks

    float lastShotTime = 0.0f;  // Time of the last shot

    /// PLAYER TANK
    glm::mat4 cannonMatrix;
    glm::mat4 turretMatrix;
    glm::mat4 projectileMatrix;
    float targetRotation = 0.f;         // Target rotation for the turret
    float turretRotationSpeed = 1.0f;   // Turret rotation speed
    /// PLAYER TANK

    int numBuildings; // Number of buildings in the scene

    float elapsedTime = 0.0f;    // Timer to track elapsed time
    bool stopEnemyMovement;      // Stop enemy movement
    bool stopGameRender;         // Stop game rendering
};

#endif // WORLD_OF_TANKS_H
