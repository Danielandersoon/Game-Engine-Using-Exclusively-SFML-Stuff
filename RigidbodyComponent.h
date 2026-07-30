#ifndef RIGIDBODY_COMPONENT_H
#define RIGIDBODY_COMPONENT_H

#include "./Component.h"
#include "./PhysicsEngine/RigidBody.h"
#include "./PhysicsEngine/Collider.h"
#include "./Scene.h"
#include "./Logger.h"

namespace GUESS::core {
    class RigidbodyComponent : public Component {
    public:
        std::unique_ptr<GUESS::physics::Rigidbody3D> rigidbody;

    public:
        void init() override {
            setName("rigidbody component");
            if (!rigidbody) {
                rigidbody = std::make_unique<GUESS::physics::Rigidbody3D>();
            }

            // Keep rigidbody transform aligned when init is called more than once.
            if (m_ownerScene) {
                auto* ownerObj = m_ownerScene->FindGameObject(owner);
                if (ownerObj) {
                    rigidbody->setPosition(ownerObj->getTransform().getPosition());
                }
            }
        }

        void update() const override {
            const auto& pos = rigidbody->getPosition();
            m_ownerScene->FindGameObject(getOwner())->getTransform().setPosition(pos);

            // Reduced debug: only log when velocity is non-zero (avoid flooding)
            auto vel = rigidbody->getVelocity();
            if (vel.x != 0.0f || vel.y != 0.0f || vel.z != 0.0f) {
                std::string msg = "RigidbodyComponent owner=" + std::to_string(getOwner()) +
                    " vel=(" + std::to_string(vel.x) + "," + std::to_string(vel.y) + "," + std::to_string(vel.z) + ")" +
                    " mass=" + std::to_string(rigidbody->getMass());
                GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, msg);
            }
        }

        void setMass(float mass) { rigidbody->setMass(mass); }
        void setFriction(float friction) { rigidbody->setFriction(friction); }
        void setRestitution(float restitution) { rigidbody->setRestitution(restitution); }

        void addForce(const GUESS::core::math::Vector3f& force) {
            rigidbody->addForce(force);
        }

        void setStatic(bool s) { rigidbody->setIsStatic(s); }
        bool isStatic() const { return rigidbody->getIsStatic(); }

        GUESS::physics::Rigidbody3D* getRigidbody() { return rigidbody.get(); }
    };
}
#endif
