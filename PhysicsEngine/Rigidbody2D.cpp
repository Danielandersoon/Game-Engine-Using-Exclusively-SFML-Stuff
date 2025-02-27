#include "./Rigidbody2D.h"

namespace GUESS::physics {
    void RigidBody2D::update(float deltaTime) {
        // Add angular physics
        angularVelocity += torque * deltaTime;
        rotation += angularVelocity * deltaTime;

        // Apply forces
        acceleration = acceleration + (forceAccum * (1.0f / mass));

        angularVelocity += torque * deltaTime;
        rotation += angularVelocity * deltaTime;

        // Get collision state from physics world instead
        bool hasCollision = false; // Physics world should provide this

        if (!hasCollision) {
            // Apply gravity when no collision
            acceleration = acceleration + (GUESS::core::math::Vector2f(0, GRAVITY * mass) * deltaTime);
        }

        // Update linear motion
        velocity = velocity + (acceleration * deltaTime);

        // Apply friction
        velocity = velocity * (1.0f - friction * deltaTime);

        // Update position
        position = position + (velocity * deltaTime);

        // Reset forces and torque
        clearForces();
        torque = 0.0f;
        acceleration = GUESS::core::math::Vector2f();
    }
}
