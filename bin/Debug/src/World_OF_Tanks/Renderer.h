#pragma once

#ifndef RENDERER_H
#define RENDERER_H

#include "components/simple_scene.h"
#include "components/camera_input.h"
#include "components/scene_input.h"
#include "components/transform.h"

#include "Camera3rdPerson.h"
#include "Transforms3D.h"

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <unordered_map>


struct EnemyTank;
struct PlayerTank;


class Renderer : public gfxc::SimpleScene  {
public:
    Renderer(
        Camera3rdPerson::Camera* camera,
        std::unordered_map<std::string, Mesh*>& meshes,
        std::unordered_map<std::string, Shader*>& shaders
    );

    /// Render a simple mesh using the specified shader and model matrix
    void RenderSimpleMesh(
        Mesh* mesh,
        Shader* shader,
        const glm::mat4& modelMatrix
    );

    /// Create a mesh with the given name, vertices, and indices
    Mesh* CreateMesh(
        const char* name,
        const std::vector<VertexFormat>& vertices,
        const std::vector<unsigned int>& indices
    );

    /// Render an enemy tank using the specified shader, model matrix, and color
    void RenderEnemyTank(
        Mesh* mesh,
        Shader* shader,
        const glm::mat4& modelMatrix,
        
        EnemyTank& enemy,
        const glm::vec3& color
    );

    /// Render player tank using the specified shader, model matrix, and color.
    void RenderPlayerTank(
        Mesh* mesh,
        Shader* shader,
        const glm::mat4& modelMatrix,

        PlayerTank& player,
        const glm::vec3& color
    );

private:
    Camera3rdPerson::Camera* camera;
    std::unordered_map<std::string, Mesh*>& meshes;
    std::unordered_map<std::string, Shader*>& shaders;
};

#endif // RENDERER_H
