#ifndef CAPSULE_COLLISION_COMPONENT_H 
#define CAPSULE_COLLISION_COMPONENT_H

#include "Component.h"
#include "../PhysicsEngine/CapsuleCollider.h"
#include "../PhysicsEngine/RigidBody.h"

namespace GUESS::core {
    class CapsuleCollisionComponent : public Component {
    private:
        std::unique_ptr<GUESS::physics::CapsuleCollider> collider;
        GUESS::physics::Rigidbody3D* rigidbody = nullptr;

    public:
        void init() override {
            setName("capsule collision component");
            collider = std::make_unique<GUESS::physics::CapsuleCollider>();
            
            // Try to get rigidbody if it exists
            rigidbody = getOwner()->getComponent<GUESS::physics::Rigidbody3D>();
            if(rigidbody) {
                rigidbody->setCollider(collider.get());
            }
        }

        void update() override {
            // Update collider transform from GameObject
            const auto& transform = getOwner()->getTransform();
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
