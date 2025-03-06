#ifndef BOX_COLLISION_COMPONENT_H
#define BOX_COLLISION_COMPONENT_H

#include <memory>
#include "./Component.h"
#include "./PhysicsEngine/RigidBody.h"
#include "./PhysicsEngine/BoxCollider.h"
#include "./RigidbodyComponent.h"
#include "./Scene.h"

namespace GUESS::core {
    class Scene;

    class BoxCollisionComponent : public Component {
    public:
        std::unique_ptr<GUESS::physics::BoxCollider> collider;
        GUESS::core::RigidbodyComponent* rigidbody;

    public:
        void init() override {
            setName("box collision component");
            collider = std::make_unique<GUESS::physics::BoxCollider>();

            GameObject* owner = m_ownerScene.get()->FindGameObject(getOwner());
            rigidbody = owner->getComponent<RigidbodyComponent>();
            if (rigidbody) {
                collider->setAttachedRigidbody(rigidbody->getRigidbody());
            }
            
        }

        void update() {
            const auto& transform = m_ownerScene.get()->FindGameObject(getOwner())->getTransform();
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
