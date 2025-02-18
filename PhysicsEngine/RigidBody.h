#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../GMath.h"

namespace GUESS::physics {
    template <typename T>
    class Collider;

    template <typename T>
    class RigidBody {
    protected:
        T position;
        T velocity;
        T acceleration;
        float mass;
        float friction;
        float restitution;
        Collider<T>* collider;
        T forceAccum;
    public:
        RigidBody(float mass, float friction, float restitution) : mass(mass), friction(friction), restitution(restitution) {};
        void applyForce(const T& force) { acceleration += force; };
        void update(float deltaTime);
        T getPosition() const { return position; };
        T  getVelocity() const { return velocity; };
        T getAcceleration() const { return acceleration; };
        float getMass() const { return mass; };
        float getFriction() const { return friction; };
        float getRestitution() const { return restitution; };
        Collider<T> getCollider() { return collider; }
        void clearForces() { forceAccum = T(); }
        void addForce(const T& force) { forceAccum = forceAccum + force; }
    };
}

#endif