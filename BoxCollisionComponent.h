#ifndef BOX_COLLISION_COMPONENT_H
#define BOX_COLLISION_COMPONENT_H

#include "./Component.h"
#include "./PhysicsEngine/BoxCollider.h"
#include "./PhysicsEngine/RigidBody.h"

namespace GUESS::core {
    class BoxCollisionComponent : public Component {
    private:
        std::unique_ptr<GUESS::physics::BoxCollider> collider;
        GUESS::physics::Rigidbody3D* rigidbody = nullptr;

    public:
        void init() override {
            setName("box collision component");
            collider = std::make_unique<GUESS::physics::BoxCollider>();
            
            // Try to get rigidbody if it exists
            rigidbody = getOwner()->getComponent<GUESS::physics::Rigidbody3D>();
            if(rigidbody) {
                rigidbody->setCollider(collider.get());
            }
        }

        void update() {
            // Update collider transform from GameObject
            const auto& transform = getOwner()->getTransform();
            collider->setPosition(transform.getPosition());
            collider->setRotation(transform.getRotation().toEuler().y);
            collider->setScale(transform.getScale());
        }

        void setDimensions(const GUESS::core::math::Vector3f& dimensions) {
            collider->setDimensions(dimensions);
        }

        void setTrigger(bool isTrigger) {
            collider->setTrigger(isTrigger); 
        }

        GUESS::physics::BoxCollider* getCollider() {
            return collider.get();
        }
    };
}
#endif
