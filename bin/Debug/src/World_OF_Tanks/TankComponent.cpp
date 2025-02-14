#include "TankComponent.h"


namespace TankComponent
{
    Mesh* TankComponent::CreateBodyMesh()
    {
        // Define tank body dimensions and vertices
        const float bodyBaseLength = 2.5f;
        const float bodyTopLength = 2.0f;
        const float bodyWidth = 1.5f;
        const float bodyHeight = 0.5f;
        const float liftAmount = 0.1f;

        // Vertices for the tank body
        std::vector<VertexFormat> bodyVertices =
        {
            // Bottom face
            VertexFormat(glm::vec3(-bodyBaseLength / 2, 0, -bodyWidth / 2), glm::vec3(0, 1, 0)),
            VertexFormat(glm::vec3(bodyBaseLength / 2, 0, -bodyWidth / 2), glm::vec3(0, 1, 0)),
            VertexFormat(glm::vec3(bodyBaseLength / 2, 0, bodyWidth / 2), glm::vec3(0, 1, 0)),
            VertexFormat(glm::vec3(-bodyBaseLength / 2, 0, bodyWidth / 2), glm::vec3(0, 1, 0)),
            // Top face
            VertexFormat(glm::vec3(-bodyTopLength / 2, bodyHeight, -bodyWidth / 2), glm::vec3(0, 1, 0)),
            VertexFormat(glm::vec3(bodyTopLength / 2, bodyHeight, -bodyWidth / 2), glm::vec3(0, 1, 0)),
            VertexFormat(glm::vec3(bodyTopLength / 2, bodyHeight, bodyWidth / 2), glm::vec3(0, 1, 0)),
            VertexFormat(glm::vec3(-bodyTopLength / 2, bodyHeight, bodyWidth / 2), glm::vec3(0, 1, 0)),
        };

        // Update the Y-coordinates of the vertices for the bottom face
        for (int i = 0; i < 4; ++i) bodyVertices[i].position.y += liftAmount;
        // Update the Y-coordinates of the vertices for the top face
        for (int i = 4; i < 8; ++i) bodyVertices[i].position.y += liftAmount;

        std::vector<unsigned int> bodyIndices =
        {
            // Bottom face
            0, 1, 2,
            2, 3, 0,
            // Top face
            4, 5, 6,
            6, 7, 4,
            // Front face
            0, 1, 5,
            5, 4, 0,
            // Back face
            2, 3, 7,
            7, 6, 2,
            // Left face
            0, 3, 7,
            7, 4, 0,
            // Right face
            1, 2, 6,
            6, 5, 1,
        };

        return createMesh("tankBody", bodyVertices, bodyIndices);
    }


    Mesh* TankComponent::CreateTurretMesh()
    {
        // Define turret dimensions and vertices
        const float bodyHeight = 0.5f;
        const float turretWidthBottom = 1.2f;
        const float turretWidthTop = 0.8f;
        const float turretHeight = 0.4f;
        const float turretLength = 1.0f;

        // Vertices for the trapezoidal tank turret
        std::vector<VertexFormat> turretVertices =
        {
            // Bottom face
            VertexFormat(glm::vec3(-turretLength / 2, bodyHeight, -turretWidthBottom / 2), glm::vec3(0.5, 1, 0.5)),
            VertexFormat(glm::vec3(turretLength / 2, bodyHeight, -turretWidthBottom / 2), glm::vec3(0.5, 1, 0.5)),
            VertexFormat(glm::vec3(turretLength / 2, bodyHeight, turretWidthBottom / 2), glm::vec3(0.5, 1, 0.5)),
            VertexFormat(glm::vec3(-turretLength / 2, bodyHeight, turretWidthBottom / 2), glm::vec3(0.5, 1, 0.5)),

            // Top face
            VertexFormat(glm::vec3(-turretLength / 2, bodyHeight + turretHeight, -turretWidthTop / 2), glm::vec3(0.5, 1, 0.5)),
            VertexFormat(glm::vec3(turretLength / 2, bodyHeight + turretHeight, -turretWidthTop / 2), glm::vec3(0.5, 1, 0.5)),
            VertexFormat(glm::vec3(turretLength / 2, bodyHeight + turretHeight, turretWidthTop / 2), glm::vec3(0.5, 1, 0.5)),
            VertexFormat(glm::vec3(-turretLength / 2, bodyHeight + turretHeight, turretWidthTop / 2), glm::vec3(0.5, 1, 0.5)),
        };

        // Define indices to form triangles for rendering
        std::vector<unsigned int> turretIndices =
        {
            // Bottom face
            0, 1, 2,    2, 3, 0,
            // Top face
            4, 5, 6,    6, 7, 4,
            // Front face
            0, 1, 5,    5, 4, 0,
            // Back face
            2, 3, 7,    7, 6, 2,
            // Left face
            0, 3, 7,    7, 4, 0,
            // Right face
            1, 2, 6,    6, 5, 1,
        };

        return createMesh("tankTurret", turretVertices, turretIndices);

    }


    Mesh* TankComponent::CreateCannonMesh()
    {
        // Define cannon dimensions
        const float bodyHeight = 0.5f;
        const float turretHeight = 0.4f;
        ///
        const float cannonLength = 1.f;
        const float cannonWidth = 0.1f;
        const float cannonHeight = 1.2f;
        const float cannonRadius = 0.05f;

        // Vertices for the tank cannon
        std::vector<unsigned int> cannonIndices;
        std::vector<VertexFormat> cannonVertices;
        const int numSegments = 16; // Number of segments to approximate the cylinder

        // Generate the vertices for the top and bottom circles of the cylinder
        for (int i = 0; i < numSegments; ++i)
        {
            float angle = (float)i / numSegments * 2.0f * M_PI;
            float x = cos(angle) * cannonRadius;
            float z = sin(angle) * cannonRadius;
            // Top circle vertex
            cannonVertices.push_back(VertexFormat(glm::vec3(x, bodyHeight + turretHeight + cannonHeight, z), glm::vec3(0.75, 0.75, 0.75)));
            // Bottom circle vertex
            cannonVertices.push_back(VertexFormat(glm::vec3(x, bodyHeight + turretHeight, z), glm::vec3(0.75, 0.75, 0.75)));
        }

        // Generate the indices for the triangles that make up the cylinder
        for (int i = 0; i < numSegments; ++i)
        {
            int nextIndex = (i + 1) % numSegments;
            // Top circle triangle
            cannonIndices.push_back(i * 2);
            cannonIndices.push_back(nextIndex * 2);
            cannonIndices.push_back(i * 2 + 1);
            // Bottom circle triangle
            cannonIndices.push_back(nextIndex * 2 + 1);
            cannonIndices.push_back(i * 2 + 1);
            cannonIndices.push_back(nextIndex * 2);
            // Side rectangles (two triangles per side)
            // 1 
            cannonIndices.push_back(i * 2);
            cannonIndices.push_back(i * 2 + 1);
            cannonIndices.push_back(nextIndex * 2 + 1);
            // 2
            cannonIndices.push_back(nextIndex * 2 + 1);
            cannonIndices.push_back(nextIndex * 2);
            cannonIndices.push_back(i * 2);
        }

        return createMesh("tankCannon", cannonVertices, cannonIndices);
    }


    glm::vec3 Rotate90_Y(glm::vec3 vertex)
    {
        // Rotation matrix for 90 degrees around the Y-axis
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1, 0));
        glm::vec4 rotatedVertex = rotationMatrix * glm::vec4(vertex, 1.0f);
        return glm::vec3(rotatedVertex.x, rotatedVertex.y, rotatedVertex.z);
    }


    Mesh* TankComponent::CreateWheel1Mesh()
    {
        // Define wheel dimensions and parameters
        const float wheelHeight = 0.3f;
        const float wheelLength = 0.5f;
        const float wheelWidth = 1.5f;
        const float largerBaseWidth = wheelWidth * 2.f;
        const float liftHeight = wheelHeight * 0.5f;

        // Vertices for the tank wheel
        std::vector<VertexFormat> wheelVertices =
        {
            // Top rectangle (unchanged in width, but lifted on OY)
            VertexFormat(Rotate90_Y(glm::vec3(-wheelLength / 2, liftHeight, -wheelWidth / 2)),
            glm::vec3(0.75, 0.75, 0.75)), // Top-left
            VertexFormat(Rotate90_Y(glm::vec3(wheelLength / 2, liftHeight, -wheelWidth / 2)),
            glm::vec3(0.75, 0.75, 0.75)),  // Top-right
            VertexFormat(Rotate90_Y(glm::vec3(wheelLength / 2, liftHeight, wheelWidth / 2)),
            glm::vec3(0.75, 0.75, 0.75)),   // Top-right back
            VertexFormat(Rotate90_Y(glm::vec3(-wheelLength / 2, liftHeight, wheelWidth / 2)),
            glm::vec3(0.75, 0.75, 0.75)), // Top-left back
            // Bottom rectangle (wider than the top and lifted on OY)
            VertexFormat(Rotate90_Y(glm::vec3(-wheelLength / 2, -wheelHeight + liftHeight, -largerBaseWidth / 2)),
            glm::vec3(0.75, 0.75, 0.75)), // Bottom-left
            VertexFormat(Rotate90_Y(glm::vec3(wheelLength / 2, -wheelHeight + liftHeight, -largerBaseWidth / 2)),
            glm::vec3(0.75, 0.75, 0.75)),  // Bottom-right
            VertexFormat(Rotate90_Y(glm::vec3(wheelLength / 2, -wheelHeight + liftHeight, largerBaseWidth / 2)),
            glm::vec3(0.75, 0.75, 0.75)),   // Bottom-right back
            VertexFormat(Rotate90_Y(glm::vec3(-wheelLength / 2, -wheelHeight + liftHeight, largerBaseWidth / 2)),
            glm::vec3(0.75, 0.75, 0.75)), // Bottom-left back
        };

        // Define indices to form triangles for rendering
        std::vector<unsigned int> wheelIndices =
        {
            // Top
            0, 1, 2,  2, 3, 0,
            // Bottom
            4, 5, 6,  6, 7, 4,
            // Sides
            0, 1, 5,  5, 4, 0,
            1, 2, 6,  6, 5, 1,
            2, 3, 7,  7, 6, 2,
            3, 0, 4,  4, 7, 3,
        };

        return createMesh("tankWheel1", wheelVertices, wheelIndices);
    }


    Mesh* TankComponent::CreateWheel2Mesh()
    {
        // Define wheel dimensions and parameters
        const float wheelHeight = 0.3f;
        const float wheelLength = 0.5f;
        const float wheelWidth = 1.5f;
        const float largerBaseWidth = wheelWidth * 2.f;
        const float liftHeight = wheelHeight * 0.5f;

        // Vertices for the tank wheel
        std::vector<VertexFormat> wheelVertices =
        {
            // Top rectangle (unchanged in width, but lifted on OY)
            VertexFormat(Rotate90_Y(glm::vec3(-wheelLength / 2, liftHeight, -wheelWidth / 2)),
            glm::vec3(0.75, 0.75, 0.75)), // Top-left
            VertexFormat(Rotate90_Y(glm::vec3(wheelLength / 2, liftHeight, -wheelWidth / 2)),
            glm::vec3(0.75, 0.75, 0.75)),  // Top-right
            VertexFormat(Rotate90_Y(glm::vec3(wheelLength / 2, liftHeight, wheelWidth / 2)),
            glm::vec3(0.75, 0.75, 0.75)),   // Top-right back
            VertexFormat(Rotate90_Y(glm::vec3(-wheelLength / 2, liftHeight, wheelWidth / 2)),
            glm::vec3(0.75, 0.75, 0.75)), // Top-left back
            // Bottom rectangle (wider than the top and lifted on OY)
            VertexFormat(Rotate90_Y(glm::vec3(-wheelLength / 2, -wheelHeight + liftHeight, -largerBaseWidth / 2)),
            glm::vec3(0.75, 0.75, 0.75)), // Bottom-left
            VertexFormat(Rotate90_Y(glm::vec3(wheelLength / 2, -wheelHeight + liftHeight, -largerBaseWidth / 2)),
            glm::vec3(0.75, 0.75, 0.75)),  // Bottom-right
            VertexFormat(Rotate90_Y(glm::vec3(wheelLength / 2, -wheelHeight + liftHeight, largerBaseWidth / 2)),
            glm::vec3(0.75, 0.75, 0.75)),   // Bottom-right back
            VertexFormat(Rotate90_Y(glm::vec3(-wheelLength / 2, -wheelHeight + liftHeight, largerBaseWidth / 2)),
            glm::vec3(0.75, 0.75, 0.75)), // Bottom-left back
        };

        // Define indices to form triangles for rendering
        std::vector<unsigned int> wheelIndices =
        {
            // Top
            0, 1, 2,  2, 3, 0,
            // Bottom
            4, 5, 6,  6, 7, 4,
            // Sides
            0, 1, 5,  5, 4, 0,
            1, 2, 6,  6, 5, 1,
            2, 3, 7,  7, 6, 2,
            3, 0, 4,  4, 7, 3,
        };

        return createMesh("tankWheel2", wheelVertices, wheelIndices);
    }
} // namepsace TankComponent
