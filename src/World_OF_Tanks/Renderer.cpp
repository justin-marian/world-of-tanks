#include "Renderer.h"
#include "EnemyTanks.h"
#include "GameConstants.h"


Renderer::Renderer(
    Camera3rdPerson::Camera* camera,
    std::unordered_map<std::string, Mesh*>& meshes,
    std::unordered_map<std::string, Shader*>& shaders
) : camera(camera), meshes(meshes), shaders(shaders)
{ /* DEFAULT EMPTY CONSTRUCTOR */ }


/// <summary>
/// Render a simple mesh using the specified shader and model matrix.
/// </summary>
/// <param name="mesh">Mesh to render.</param>
/// <param name="shader">Shader to use for rendering.</param>
/// <param name="modelMatrix">Model transformation matrix.</param>
void Renderer::RenderSimpleMesh(
    Mesh* mesh,
    Shader* shader,
    const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID()) return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Get shader location for uniform mat4 "Model"
    int modelLocation = glGetUniformLocation(shader->program, "Model");
    // Set shader uniform "Model" to modelMatrix
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Get shader location for uniform mat4 "View"
    int viewLocation = glGetUniformLocation(shader->program, "View");
    // Set shader uniform "View" to viewMatrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Get shader location for uniform mat4 "Projection"
    int projectionLocation = glGetUniformLocation(shader->program, "Projection");
    // Set shader uniform "Projection" to projectionMatrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    float timeLocation = glGetUniformLocation(shader->program, "Time");
    glUniform1f(timeLocation, Engine::GetElapsedTime());

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/// <summary>
/// Render an enemy tank using the specified shader and model matrix, applying a color.
/// </summary>
/// <param name="mesh">Mesh of the enemy tank.</param>
/// <param name="shader">Shader to use for rendering.</param>
/// <param name="modelMatrix">Model transformation matrix.</param>
/// <param name="enemy">Enemy tank data.</param>
/// <param name="color">Color to apply to the tank.</param>
void Renderer::RenderEnemyTank(
    Mesh* mesh,
    Shader* shader,
    const glm::mat4& modelMatrix,
    EnemyTank& enemy,
    const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID()) return;

    glUseProgram(shader->program);

    glUniform3fv(glGetUniformLocation(shader->program, "ObjectColor"), 1, glm::value_ptr(color));

    glUniformMatrix4fv(glGetUniformLocation(shader->program, "Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "View"), 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "Projection"), 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));

    glUniform1f(glGetUniformLocation(shader->program, "Health"), enemy.health);

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}


/// <summary>
/// Render player tank using the specified shader and model matrix, applying a color.
/// </summary>
/// <param name="mesh">Mesh of the player tank.</param>
/// <param name="shader">Shader to use for rendering.</param>
/// <param name="modelMatrix">Model transformation matrix.</param>
/// <param name="player">Player tank data.</param>
/// <param name="color">Color to apply to the tank.</param>
void Renderer::RenderPlayerTank(
    Mesh* mesh,
    Shader* shader,
    const glm::mat4& modelMatrix,
    PlayerTank& player,
    const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID()) return;

    glUseProgram(shader->program);

    glUniform3fv(glGetUniformLocation(shader->program, "ObjectColor"), 1, glm::value_ptr(color));
    
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "View"), 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "Projection"), 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));

    glUniform1f(glGetUniformLocation(shader->program, "Health"), player.health);

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}


/// <summary>
/// Create a mesh with the given name, vertices, and indices, then returns a pointer to the mesh.
/// Set up the vertex array object (VAO), vertex buffer object (VBO), and the index buffer object (IBO).
/// </summary>
/// <param name="name">Name of the mesh to create.</param>
/// <param name="vertices">Vector of vertices defining the mesh geometry.</param>
/// <param name="indices">Vector of unsigned integers defining the mesh topology.</param>
/// <returns>Pointer to the created Mesh object.</returns>
Mesh* Renderer::CreateMesh(
    const char* name,
    const std::vector<VertexFormat>& vertices,
    const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
        sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
        sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 
        sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}
