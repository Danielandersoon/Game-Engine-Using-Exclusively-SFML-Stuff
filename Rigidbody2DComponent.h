#ifndef RIGIDBODY2D_COMPONENT_H
#define RIGIDBODY2D_COMPONENT_H

#include "./Component.h"
#include "./PhysicsEngine/RigidBody2D.h"

namespace GUESS::core {
    class Rigidbody2DComponent : public Component {
    private:
        std::unique_ptr<GUESS::physics::RigidBody2D> rigidbody;

    public:
        void init() override {
            setName("rigidbody2d component");
            rigidbody = std::make_unique<GUESS::physics::RigidBody2D>();
        }

        void update() {
            // Update transform from physics simulation
            const auto& pos = rigidbody->getPosition();
            getOwner()->getTransform().setPosition(GUESS::core::math::Vector3f(pos.x, pos.y, 0));
            getOwner()->getTransform().setRotation(GUESS::core::math::Vector3f(0, 0, rigidbody->getRotation()));
        }

        void setMass(float mass) { rigidbody->setMass(mass); }
        void setFriction(float friction) { rigidbody->setFriction(friction); }
        void setRestitution(float restitution) { rigidbody->setRestitution(restitution); }

        void addForce(const GUESS::core::math::Vector2f& force) {
            rigidbody->addForce(force);
        }

        void addTorque(float torque) {
            rigidbody->addTorque(torque);
        }

        GUESS::physics::RigidBody2D* getRigidbody() { return rigidbody.get(); }
    };
}
#endif
