#ifndef RIGIDBODY_2D_H
#define RIGIDBODY_2D_H

#include "RigidBody.h"

namespace GUESS::physics {
    class RigidBody2D : public RigidBody<GUESS::core::math::Vector2f> {
    public:
        RigidBody2D(float mass = 1.0f, float friction = 0.5f, float restitution = 0.5f) 
            : RigidBody<GUESS::core::math::Vector2f>(mass, friction, restitution) {}

        void addTorque(float torque);
        void setRotation(float angle);
        float getRotation() const;
        float getAngularVelocity() const;

    private:
        float rotation = 0.0f;
        float angularVelocity = 0.0f;
        float torque = 0.0f;
    };
}

#endif