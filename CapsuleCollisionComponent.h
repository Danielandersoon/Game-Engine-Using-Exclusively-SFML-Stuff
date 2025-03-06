#ifndef CAPSULE_COLLISION_COMPONENT_H 
#define CAPSULE_COLLISION_COMPONENT_H

#include "./Component.h"
#include "./RigidbodyComponent.h"
#include "./PhysicsEngine/CapsuleCollider.h"
#include "./PhysicsEngine/RigidBody.h"
#include "./Scene.h"

namespace GUESS::core {
    class CapsuleCollisionComponent : public Component {
    public:
        std::unique_ptr<GUESS::physics::CapsuleCollider> collider;
        GUESS::core::RigidbodyComponent* rigidbody = nullptr;

    public:
        void init() override {
            setName("capsule collision component");
            collider = std::make_unique<GUESS::physics::CapsuleCollider>();

            auto owner = m_ownerScene->FindGameObject(getOwner());
            rigidbody = owner->getComponent<GUESS::core::RigidbodyComponent>();
            if (rigidbody) {
                rigidbody->getRigidbody()->setCollider(collider.get());
            }
        }

        void update() {
            auto owner = m_ownerScene->FindGameObject(getOwner());
            const auto& transform = owner->getTransform();
            collider->setPosition(transform.getPosition());
            collider->setRotation(transform.getRotation().toEuler().y);
            collider->setScale(transform.getScale());
        }

        void setRadius(float radius) {
            collider->setRadius(radius);
        }

        void setHeight(float height) {
            collider->setHeight(height);
        }

        void setTrigger(bool isTrigger) {
            collider->setTrigger(isTrigger);
        }

        GUESS::physics::CapsuleCollider* getCollider() {
            return collider.get();
        }
    };
}
#endif
