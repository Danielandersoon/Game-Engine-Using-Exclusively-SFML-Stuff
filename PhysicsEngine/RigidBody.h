#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../GMath.h"
#include "./PhysicsConst.h"

namespace GUESS::physics {
    template <typename T>
    class Collider;

    template <typename T>
    class RigidBody {
    protected:
        T position = T();
        T velocity = T();
        T acceleration = T();
        float mass = 1.0f;
        float friction = 0.2f;
        float restitution = 0.8f;
        Collider<T>* collider_ptr;
        T forceAccum = T();
    public:
        RigidBody() = default;
        explicit RigidBody(float mass, float friction = 0.2f, float restitution = 0.8f, Collider<T>* collider_ptr_in = nullptr) 
            : RigidBody<T>(mass, friction, restitution, collider_ptr_in) {};
        void applyForce(const T& force) { acceleration = acceleration + force; };
        T getPosition() const { return position; };
        T  getVelocity() const { return velocity; };
        T getAcceleration() const { return acceleration; };
        void setPosition(float position) { this->position = position; };
        void setVelocity(T velocity) { this->velocity = velocity; };
        void setAcceleration(float acceleration) { this->acceleration = acceleration; };
        float getMass() const { return mass; };
        float getFriction() const { return friction; };
        float getRestitution() const { return restitution; };
        void setMass(float mass) { this->mass = mass; };
        void setFriction(float friction) { this->friction = friction; };
        void setRestitution(float  restitution) { this->restitution = restitution; };
        Collider<T>* getCollider() { return collider_ptr; }
        void setCollider(Collider<T>* col) { collider_ptr = col; }
        void clearForces() { forceAccum = T(); }
        void addForce(const T& force) { forceAccum = forceAccum + force; }
        void update(float deltaTime) {
            acceleration = acceleration + (forceAccum * (1.0f / mass));

            if (!collider_ptr->checkCollision(*collider_ptr)) {
                // No collisions, just apply gravity ontop of existing accelaration.
                acceleration = acceleration + ((mass * GRAVITY) * deltaTime);
            }

            velocity = velocity + (acceleration * deltaTime);
            position = position + (velocity * deltaTime);

            // Clear forces for next frame
            clearForces();

            // Reset acceleration
            acceleration = T();
        }
    };

    using Rigidbody3D = RigidBody<GUESS::core::math::Vector3f>;
}

#endif