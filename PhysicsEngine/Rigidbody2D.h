#ifndef RIGIDBODY_2D_H
#define RIGIDBODY_2D_H

#include "./Collider.h"
#include "./RigidBody.h"

namespace GUESS::physics {
    class RigidBody2D : public RigidBody<GUESS::core::math::Vector2f> {
    public:
        void update(float deltaTime);

        void addTorque(float torque) { this->torque = torque; };
        void setRotation(float angle) { rotation = angle; };
        float getRotation() const { return rotation; };
        float getAngularVelocity() const { return angularVelocity; };

    private:
        GUESS::core::math::Vector2f position = GUESS::core::math::Vector2f();
        GUESS::core::math::Vector2f velocity = GUESS::core::math::Vector2f();
        GUESS::core::math::Vector2f acceleration = GUESS::core::math::Vector2f();
        float mass = 1.0f;
        float friction = 0.2f;
        float restitution = 0.8f;
        Collider<GUESS::core::math::Vector2f>* collider_ptr;
        GUESS::core::math::Vector2f forceAccum = GUESS::core::math::Vector2f();
        float rotation = 0.0f;
        float angularVelocity = 0.0f;
        float torque = 0.0f;
    };
}

#endif