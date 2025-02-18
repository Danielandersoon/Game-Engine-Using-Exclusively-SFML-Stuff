#include "RigidBody.h"

#include "Collider.h"
#include "PhysicsConst.h"

namespace GUESS::physics {
    template <typename T>
    void RigidBody<T>::update(float deltaTime) {
        acceleration += forceAccum * (1.0f / mass);

        if (!collider.checkCollision()) {
            // No collisions, just apply gravity ontop of existing accelaration.
            acceleration += (mass * GRAVITY) * deltaTime;
        }

        velocity += acceleration * deltaTime;
        position += velocity * deltaTime;

        // Clear forces for next frame
        clearForces();

        // Reset acceleration
        acceleration = T();
    }

}