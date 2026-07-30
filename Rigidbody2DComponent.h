#ifndef RIGIDBODY2D_COMPONENT_H
#define RIGIDBODY2D_COMPONENT_H

#include "./Component.h"
#include "./PhysicsEngine/Rigidbody2D.h"
#include "./PhysicsEngine/Collider.h"
#include "./Scene.h"

namespace GUESS::core {
    class Rigidbody2DComponent : public Component {
    public:
        std::unique_ptr<GUESS::physics::RigidBody2D> rigidbody;

    public:
        void init() override {
            setName("rigidbody component");
            rigidbody = std::make_unique<GUESS::physics::RigidBody2D>();
        }

        void update() {
            const auto& pos = rigidbody->getPosition();
            m_ownerScene->FindGameObject(owner)->getTransform().setPosition(GUESS::core::math::Vector3f(pos.x, pos.y, 0.0f));
        }

        void setMass(float mass) { rigidbody->setMass(mass); }
        void setFriction(float friction) { rigidbody->setFriction(friction); }
        void setRestitution(float restitution) { rigidbody->setRestitution(restitution); }

        void addForce(const GUESS::core::math::Vector2f& force) {
            rigidbody->addForce(force);
        }

        GUESS::physics::RigidBody2D* getRigidbody() { return rigidbody.get(); }
    };
}
#endif
