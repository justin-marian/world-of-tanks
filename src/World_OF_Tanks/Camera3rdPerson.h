#pragma once

#ifndef CAMERA_3RD_PERSON_H
#define CAMERA_3RD_PERSON_H

#include "utils/glm_utils.h"
#include "utils/math_utils.h"


namespace Camera3rdPerson
{
    class Camera
    {
    public:
        float distanceToTarget;
        glm::vec3 position;         // Current Camera Position
        glm::vec3 forward;          // Direction Vector Forward 
        glm::vec3 right;            // Direction Vector Right   
        glm::vec3 up;               // Direction Vector Up      
        glm::vec3 target;           // The target the camera should look at

        Camera()
        {
            distanceToTarget = 2.f;
            position = glm::vec3(0.0f, 2.0f, 5.0f);
            forward = glm::vec3(0.0f, 0.0f, -1.0f);
            up = glm::vec3(0.0f, 1.0f, 0.0f);
            right = glm::vec3(1.0f, 0.0f, 0.0f);
            target = glm::vec3(0.0f);
        }

        Camera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
        { 
            Set(position, center, up); 
        }
        ~Camera() { /* Default Empty Destructor */ }

        // Update camera
        void Set(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
        {
            this->position = position;
            forward = glm::normalize(center - position);
            right = glm::cross(forward, up);
            this->up = glm::cross(right, forward);
        }

        void MoveForward(float distance)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
            position += dir * distance;
        }

        void TranslateForward(float distance)
        {
            position += glm::normalize(forward) * distance;
        }
        // Translate the camera using the `up` vector.
        void TranslateUpward(float distance)
        {
            position += glm::normalize(up) * distance;
        }
        // Translate the camera using the `right` vector.
        void TranslateRight(float distance)
        {
            position += glm::normalize(right) * distance;
        }

        void RotateFirstPerson_OX(float angle)
        {
            glm::vec4 rotationMatrix = glm::rotate(glm::mat4(1.f), angle, right) * glm::vec4(forward, 0);
            forward = glm::normalize(glm::vec3(rotationMatrix));
            up = glm::cross(right, forward);
        }

        void RotateFirstPerson_OY(float angle)
        {
            glm::vec4 rotationMatrix = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 0);
            forward = glm::normalize(glm::vec3(rotationMatrix));
            rotationMatrix = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 0);
            right = glm::normalize(glm::vec3(rotationMatrix));
            up = glm::cross(right, forward);
        }

        void RotateFirstPerson_OZ(float angle)
        {
            glm::vec4 rotationMatrix = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1);
            right = glm::normalize(glm::vec3(rotationMatrix));
            rotationMatrix = glm::rotate(glm::mat4(1.f), angle, forward) * glm::vec4(up, 0);
            forward = glm::normalize(glm::vec3(rotationMatrix));
            up = glm::cross(right, forward);
        }

        /// distanceToTarget ~ (translation distance)

        // Rotate the camera in 3RD Person around the OY axis.
        void RotateThirdPerson_OX(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OX(angle);
            TranslateForward(-distanceToTarget);

        }
        // Rotate the camera in 3RD Person around the OY axis.
        void RotateThirdPerson_OY(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OY(angle);
            TranslateForward(-distanceToTarget);

        }
        // Rotate the camera in 3RD Person around the OZ axis.
        void RotateThirdPerson_OZ(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OZ(angle);
            TranslateForward(-distanceToTarget);
        }

        // Target Position Camera
        glm::vec3 GetTargetPosition()
        {
            return position + forward * distanceToTarget;
        }
    };
}   // namespace Camera3rdPerson

#endif  // CAMERA_3RD_PERSON_H
