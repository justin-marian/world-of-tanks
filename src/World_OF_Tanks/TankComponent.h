#pragma once

#ifndef TANK_COMPONENT_H
#define TANK_COMPONENT_H

#include "components/simple_scene.h"
#include "GameConstants.h"

#include <string>
#include <vector>
#include <functional>


namespace TankComponent
{
    class TankComponent
    {
    public:
        // !!!TYPE FUNCTION THAT CREATES MESHES!!! //
        using MeshCreatorFunc =
            std::function<Mesh* (
                const char*,
                const std::vector<VertexFormat>&,
                const std::vector<unsigned int>&
                )>;

        // Constructor that takes a MeshCreatorFunc
        explicit TankComponent(MeshCreatorFunc meshCreator) :
            createMesh(std::move(meshCreator)) {}

        // Functions to create different parts of a tank
        /// BODY
        Mesh* CreateBodyMesh();
        /// TURRET
        Mesh* CreateTurretMesh();
        /// CANNON
        Mesh* CreateCannonMesh();
        /// WHEEL1
        Mesh* CreateWheel1Mesh();
        /// WHEEL2
        Mesh* CreateWheel2Mesh();

    private:
        MeshCreatorFunc createMesh;
    };
} // namespace TankComponent

#endif // TANK_COMPONENT_H
