#include "World_OF_Tanks.h"
#include "Transforms3D.h"
#include "GameConstants.h"
#include "TankComponent.h"
#include "Renderer.h"

#include <utility>
#include <random>
#include <vector>
#include <string>
#include <iostream>
#include <map>

using namespace std;


World_OF_Tanks::World_OF_Tanks()
    : tankComponent(TankComponent::TankComponent(
        [this](const char* name,
            const std::vector<VertexFormat>& vertices,
            const std::vector<unsigned int>& indices) -> Mesh* {
                return renderer->CreateMesh(name, vertices, indices);
        })),
    numBuildings(0), gameInit(this),
    polygonMode(GL_FILL), resolution(800, 600), modelMatrix(glm::mat4(1.0f)),
    cannonMatrix(glm::mat4(1.0f)), projectileMatrix(glm::mat4(1.0f)), projectionMatrix(glm::mat4(1.0f)),
    stopEnemyMovement(false), stopGameRender(false)
{
    // Initialize the renderer with camera, meshes, and shaders
    renderer = new Renderer(&camera, meshes, shaders);

    // Create tank components using TankComponent methods
    Mesh* bodyMesh = tankComponent.CreateBodyMesh();
    Mesh* turretMesh = tankComponent.CreateTurretMesh();
    Mesh* cannonMesh = tankComponent.CreateCannonMesh();
    Mesh* wheelMesh1 = tankComponent.CreateWheel1Mesh();
    Mesh* wheelMesh2 = tankComponent.CreateWheel2Mesh();
}


World_OF_Tanks::~World_OF_Tanks() {}


void World_OF_Tanks::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    /// SKY COLOR RGBA (0.043, 0.337, 0.529, 1)
    glClearColor(0.043, 0.337, 0.529, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Get window resolution and set viewport accordingly
    resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void World_OF_Tanks::FrameEnd()
{
    // Restore the main viewport before drawing the coordinate system
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


/// <summary>
/// Initializes the game world, including setting up the:
/// projection matrices, loading models and shaders, 
/// creating the camera, and initializing the game elements (TANKS,BUILDINGS).
/// </summary>
void World_OF_Tanks::Init()
{
    srand(static_cast<unsigned int>(time(NULL)));

    /// MESHES LOADING
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    /// MESHES LOADING

    /// SHADERS LOADING
    {
        Shader* planeShader = new Shader("Plane");
        planeShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::PATH_PROJECT,
                                         "World_OF_Tanks", "shaders", "VertexShaderPlane.glsl"), GL_VERTEX_SHADER);
        planeShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::PATH_PROJECT,
                                         "World_OF_Tanks", "shaders", "FragmentShaderPlane.glsl"), GL_FRAGMENT_SHADER);
        planeShader->CreateAndLink();
        shaders[planeShader->GetName()] = planeShader;
    }
    {
        Shader* buildingShader = new Shader("Building");
        buildingShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::PATH_PROJECT,
                                            "World_OF_Tanks", "shaders", "VertexShaderBuilding.glsl"), GL_VERTEX_SHADER);
        buildingShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::PATH_PROJECT,
                                            "World_OF_Tanks", "shaders", "FragmentShaderBuilding.glsl"), GL_FRAGMENT_SHADER);
        buildingShader->CreateAndLink();
        shaders[buildingShader->GetName()] = buildingShader;
    }
    {
        Shader* tankShader = new Shader("TankEnemy");
        tankShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::PATH_PROJECT,
                                        "World_OF_Tanks", "shaders", "VertexShaderEnemy.glsl"), GL_VERTEX_SHADER);
        tankShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::PATH_PROJECT,
                                        "World_OF_Tanks", "shaders", "FragmentShaderEnemy.glsl"), GL_FRAGMENT_SHADER);
        tankShader->CreateAndLink();
        shaders[tankShader->GetName()] = tankShader;
    }
    {
        Shader* tankShader = new Shader("TankPlayer");
        tankShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::PATH_PROJECT,
            "World_OF_Tanks", "shaders", "VertexShaderPlayer.glsl"), GL_VERTEX_SHADER);
        tankShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::PATH_PROJECT,
            "World_OF_Tanks", "shaders", "FragmentShaderPlayer.glsl"), GL_FRAGMENT_SHADER);
        tankShader->CreateAndLink();
        shaders[tankShader->GetName()] = tankShader;
    }
    /// SHADERS LOADING

    // Sets the resolution of the small viewport
    resolution = window->GetResolution();

    gameInit.InitializeBuildings();
    gameInit.InitializeEnemyTanks();
}


/// <summary>
/// RENDER ALL THE SCNEE OBJECTS PLAYER, ENEMIES, BUILDINGS AND PLANE
/// </summary>
/// <param name="viewMatrix">View matrix</param>
/// <param name="projectionMatrix">Projection matrix</param>
void World_OF_Tanks::RenderScene(
    const glm::mat4& viewMatrix,
    const glm::mat4& projectionMatrix)
{
    /// TANK PLAYER
    {
        Shader* shader = shaders["TankPlayer"];
        shader->Use();

        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera.GetTargetPosition()));
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        // Render tank body
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = Transforms3D::Translate(player.position.x, player.position.y, player.position.z);
        modelMatrix = modelMatrix * Transforms3D::RotateOY(player.trajectoryAngle);

        player.cannonAngle = player.trajectoryAngle;
        renderer->RenderPlayerTank(meshes["tankBody"], shader, modelMatrix, player, bodyColor);

        // Turret positioned on the body
        turretMatrix = modelMatrix * Transforms3D::Translate(0.0f, 0.1f, 0.0f);
        turretMatrix = turretMatrix * Transforms3D::RotateOY(player.turretRotation);
        renderer->RenderPlayerTank(meshes["tankTurret"], shader, turretMatrix, player, turretColor);

        // Cannon positioned at the front of the turret
        cannonMatrix = turretMatrix * Transforms3D::Translate(0.5, cannonHeight / 2 + 0.2, 0);
        cannonMatrix = cannonMatrix * Transforms3D::RotateOZ(M_PI / 2);
        renderer->RenderPlayerTank(meshes["tankCannon"], shader, cannonMatrix, player, cannonColor);

        glm::mat4 wheelMatrixLeft = modelMatrix * Transforms3D::Translate(0, liftHeight, -wheelOffsetFromCenter);
        renderer->RenderPlayerTank(meshes["tankWheel1"], shader, wheelMatrixLeft, player, wheelColor);

        // Right side wheels
        glm::mat4 wheelMatrixRight = modelMatrix * Transforms3D::Translate(0, liftHeight, wheelOffsetFromCenter);
        renderer->RenderPlayerTank(meshes["tankWheel2"], shader, wheelMatrixRight, player, wheelColor);
    }
    /// TANK PLAYER

    float wheelWidth = 1.2f;
    float largerBaseWidth = wheelWidth_ENEMY * 1.2f;
    float wheelOutwardOffset = largerBaseWidth / 2;

    for (auto& enemy : enemies)
    {
        if (!enemy.isRenderable) continue;

        glm::mat4 tankModelMatrix = viewMatrix;
        tankModelMatrix = glm::translate(tankModelMatrix, enemy.position);

        // Sinking effect if the tank is destroyed
        if (enemy.isDestroyed) tankModelMatrix = glm::translate(tankModelMatrix, glm::vec3(0, -enemy.sinkDepth, 0));

        tankModelMatrix = glm::rotate(tankModelMatrix, enemy.rotation, glm::vec3(0, 1, 0));

        Shader* shader = shaders["TankEnemy"];
        shader->Use();
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        // Render tank body
        renderer->RenderEnemyTank(meshes["tankBody"], shader, tankModelMatrix, enemy, enemyBodyColor);

        // Render turret (positioned on the body)
        glm::mat4 turretMatrix = tankModelMatrix * Transforms3D::Translate(0.0f, 0.1f, 0.0f) * Transforms3D::RotateOY(enemy.turretRotation - enemy.rotation);
        renderer->RenderEnemyTank(meshes["tankTurret"], shader, turretMatrix, enemy, enemyTurretColor);

        // Render cannon (positioned at the front of the turret)
        glm::mat4 cannonMatrix = turretMatrix * Transforms3D::Translate(0.5, cannonHeight / 2 + 0.2, 0);
        cannonMatrix = cannonMatrix * Transforms3D::RotateOZ(M_PI / 2);
        renderer->RenderEnemyTank(meshes["tankCannon"], shader, cannonMatrix, enemy, enemyCannonColor);

        // Left side wheels
        glm::mat4 wheelMatrixLeft = tankModelMatrix * glm::translate(viewMatrix, glm::vec3(0, liftHeight, -wheelOutwardOffset));
        renderer->RenderEnemyTank(meshes["tankWheel1"], shader, wheelMatrixLeft, enemy, enemyWheelColor);

        // Right side wheels
        glm::mat4 wheelMatrixRight = tankModelMatrix * glm::translate(viewMatrix, glm::vec3(0, liftHeight, wheelOutwardOffset));
        renderer->RenderEnemyTank(meshes["tankWheel2"], shader, wheelMatrixRight, enemy, enemyWheelColor);
    }
 
    /// PROJECTILES
    for (const auto& projectile : projectiles)
    {
        Shader* shader = shaders["VertexColor"];
        shader->Use();
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


        glm::mat4 modelMatrix = glm::translate(viewMatrix, projectile.position);
        modelMatrix = modelMatrix * Transforms3D::Scale(projectile.radius, projectile.radius, projectile.radius);
        renderer->RenderSimpleMesh(meshes["sphere"], shader, modelMatrix);
    }

    /// PLANE HORIZONTAL
    {
        Shader* shader = shaders["Plane"];
        shader->Use();
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


        glm::mat4 modelMatrix = viewMatrix;
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
        renderer->RenderSimpleMesh(meshes["plane"], shader, modelMatrix);
    }

    /// BUILDINGS
    for (int i = 0; i < numBuildings; i++)
    {
        Shader* shader = shaders["Building"];
        shader->Use();
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        string name = "cube" + to_string(i);
        renderer->RenderSimpleMesh(meshes[name.c_str()], shader, viewMatrix);
    }
}


void World_OF_Tanks::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);

    elapsedTime += deltaTimeSeconds;
    // Check if enemy movement should stop after 1 minute
    stopEnemyMovement = elapsedTime >= 60.0f;
    // Check if game rendering should stop after 70 seconds
    stopGameRender = elapsedTime >= 70.f;

    std::cout << "ELAPSED TIME: " << elapsedTime << std::endl;
    // Check if 1 minute has passed
    if (stopEnemyMovement)
    {
        projectiles.clear();
        std::cout << "!GAME ENDED!" << std::endl;
    }
    // Check for game over condition if player's health reaches 0
    if (stopGameRender)
    {
        std::cout << "!CLOSED GAME!" << std::endl;
        window->Close();
    }
    // Check for game over condition
    if (player.health <= 0 && !stopEnemyMovement)
    {
        stopEnemyMovement = true;
        projectiles.clear();
        std::cout << "!GAME OVER! PLAYER DESTROYED." << std::endl;
    }

    // Initialize view and projection matrices for rendering
    glm::mat4 viewMatrix = glm::mat4(1);
    glm::mat4 projectionMatrix = this->projectionMatrix;

    // Render the main scene using perspective projection
    RenderScene(viewMatrix, projectionMatrix);

    /// Update game elements and collisions
    ///
    Projectiles::UpdateProjectilesPlayerCollision(projectiles, player, damage);
    Projectiles::UpdateProjectileMovementsAndCollisions(projectiles, enemies, damage, deltaTimeSeconds);
    ///
    EnemyTanks::UpdateEnemyMovement(enemies, player, stopEnemyMovement, deltaTimeSeconds, attackRange,
                                    fireRate, fireAlignmentThreshold, turretRotationSpeed, targetRotation, projectiles, buildings);
    EnemyTanks::UpdateSinkingTanks(enemies, deltaTimeSeconds);
    EnemyTanks::UpdateTankCollisions(enemies, player);
    EnemyTanks::UpdateTankCollisionsWithBuildings(enemies, buildings);
    ///
    Buildings::UpdateTankBuildingCollision(buildings, player);
    Buildings::UpdateProjectileBuildingCollisiong(buildings, projectiles, deltaTimeSeconds);
}


void World_OF_Tanks::OnInputUpdate(float deltaTime, int mods)
{   
    /// Tank PLAYER movement
    if (!stopEnemyMovement)
    {
        // Move the player forward
        if (window->KeyHold(GLFW_KEY_W)) { 
            player.position += glm::vec3(
                cos(player.trajectoryAngle + M_PI) / 20,
                0,
                -sin(player.trajectoryAngle + M_PI) / 20);
        }
        // Move the player backward
        if (window->KeyHold(GLFW_KEY_S))
        {
            player.position += glm::vec3(
                -cos(player.trajectoryAngle + M_PI) / 20,
                0,
                sin(player.trajectoryAngle + M_PI) / 20);
        }
        // Rotate the player's trajectory to the left
        if (window->KeyHold(GLFW_KEY_A))
        {
            player.trajectoryAngle += deltaTime;
        }
        // Rotate the player's trajectory to the right
        if (window->KeyHold(GLFW_KEY_D))
        {
            player.trajectoryAngle -= deltaTime;
        }
    }
    /// Turret PLAYER rotation
    if (window->KeyHold(GLFW_KEY_Q)) player.turretRotation += deltaTime;
    if (window->KeyHold(GLFW_KEY_E)) player.turretRotation -= deltaTime;
}


void World_OF_Tanks::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Check if enemy movement is not stopped and also ensure that
    // enough time has passed since the last shot (2 seconds in this case)
    if (!stopEnemyMovement)
    {
        if (button == GLFW_MOUSE_BUTTON_2 && (Engine::GetElapsedTime() - lastShotTime >= 2.0f))
        {
            Projectile newProjectile;
            float projectileSpeed = 10;
            float cannonLength = 1.0f;

            // Transformation matrix for the turret and cannon, necessary to render the projectile
            // from the tip of the cannon
            turretMatrix = turretMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(-2, 0.75, 0));
            cannonMatrix = turretMatrix * Transforms3D::RotateOY(3 * M_PI / 2);

            /// Initial position and velocity
            // Position part of the matrix
            glm::vec3 worldCannonTip = glm::vec3(cannonMatrix[3]);
            // Forward direction
            glm::vec3 velocityDirection = glm::normalize(glm::vec3(cannonMatrix[2]));

            // Set the projectile's position and velocity
            newProjectile.radius = 0.1;
            newProjectile.position = worldCannonTip;
            newProjectile.velocity = velocityDirection * projectileSpeed;

            // Update the time of the last shot
            lastShotTime = Engine::GetElapsedTime();

            // Add the new projectile to the list
            projectiles.push_back(newProjectile);
        }
    }
}


void World_OF_Tanks::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {}
void World_OF_Tanks::OnKeyPress(int key, int mods) {}
void World_OF_Tanks::OnKeyRelease(int key, int mods) {}
void World_OF_Tanks::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}
void World_OF_Tanks::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}
void World_OF_Tanks::OnWindowResize(int width, int height) {}
