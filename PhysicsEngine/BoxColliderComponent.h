#ifndef BOX_COLLIDER_COMPONENT_H
#define BOX_COLLIDER_COMPONENT_H

#include "../Component.h"
#include "BoxCollider.h"
#include "../Scene.h"
#include "../RigidbodyComponent.h"
#include "../Logger.h"

namespace GUESS::core {
    class BoxColliderComponent : public Component {
    public:
        bool isStatic = false;
        bool triggerEnabled = false;
        GUESS::core::math::Vector3f dimensions;
        std::unique_ptr<GUESS::physics::BoxCollider> collider;

        BoxColliderComponent(const GUESS::core::math::Vector3f& dims = GUESS::core::math::Vector3f(1.0f,1.0f,1.0f),
                             bool isStaticIn = false,
                             bool isTriggerIn = false)
            : isStatic(isStaticIn), triggerEnabled(isTriggerIn), dimensions(dims) {}

        void init() override {
            setName("box collider component");
            if (!collider) {
                collider = std::make_unique<GUESS::physics::BoxCollider>(dimensions);
            }
            collider->setTrigger(triggerEnabled);

            // Set initial transform
            auto* ownerObj = m_ownerScene->FindGameObject(owner);
            if (ownerObj) {
                // Interpret 'dimensions' passed to this component as the desired world-space size.
                // Store collider->dimensions in local/object space so that collider uses scale correctly:
                auto ownerScale = ownerObj->getTransform().getScale();
                GUESS::core::math::Vector3f storedDims = dimensions;
                // Avoid division by zero
                if (ownerScale.x != 0.0f && ownerScale.y != 0.0f && ownerScale.z != 0.0f) {
                    storedDims = GUESS::core::math::Vector3f(dimensions.x / ownerScale.x,
                                                            dimensions.y / ownerScale.y,
                                                            dimensions.z / ownerScale.z);
                }
                collider->setDimensions(storedDims);
                collider->setPosition(ownerObj->getTransform().getPosition());
                collider->setScale(ownerScale);
            }

            // Attach to rigidbody if present
            if (ownerObj) {
                auto* rbComp = ownerObj->getComponent<RigidbodyComponent>();
                if (rbComp && rbComp->getRigidbody()) {
                    // Ensure rigidbody position matches owner
                    rbComp->getRigidbody()->setPosition(ownerObj->getTransform().getPosition());
                    rbComp->getRigidbody()->setOrientation(ownerObj->getTransform().getRotation());
                    rbComp->getRigidbody()->setCollider(collider.get());
                    collider->setAttachedRigidbody(rbComp->getRigidbody());

                    // Calculate inertia tensor using WORLD-SPACE dimensions
                    GUESS::core::math::Vector3f worldDimensions = collider->getDimensions() * collider->getScale();
                    rbComp->getRigidbody()->calculateBoxInertia(worldDimensions);

                    // If this collider is intended to be static, mark the existing rb
                    if (isStatic) {
                        rbComp->setStatic(true);
                        rbComp->getRigidbody()->setMass(1e8f);
                    }

                    // Add rigidbody to physics world
                    if (m_ownerScene) {
                        m_ownerScene->getPhysicsWorld()->addBody(rbComp->getRigidbody());
                    }
                }
                else if (isStatic) {
                    // For static objects without an explicit rigidbody, create one with a very large mass
                    auto createdRbComp = ownerObj->addComponent<RigidbodyComponent>();
                    createdRbComp->init();
                    createdRbComp->getRigidbody()->setMass(1e8f);
                    createdRbComp->setStatic(true);
                    createdRbComp->getRigidbody()->setPosition(ownerObj->getTransform().getPosition());
                    createdRbComp->getRigidbody()->setOrientation(ownerObj->getTransform().getRotation());
                    createdRbComp->getRigidbody()->setCollider(collider.get());
                    collider->setAttachedRigidbody(createdRbComp->getRigidbody());
                    GUESS::core::math::Vector3f worldDimensions = collider->getDimensions() * collider->getScale();
                    createdRbComp->getRigidbody()->calculateBoxInertia(worldDimensions);
                    if (m_ownerScene) {
                        m_ownerScene->getPhysicsWorld()->addBody(createdRbComp->getRigidbody());
                    }
                }
            }
        }

        void update() const override {
            if (!m_ownerScene) return;
            auto* ownerObj = m_ownerScene->FindGameObject(owner);
            if (!ownerObj || !collider) return;

            // Keep collider aligned with GameObject transform (position, rotation, scale)
            collider->setPosition(ownerObj->getTransform().getPosition());
            collider->setScale(ownerObj->getTransform().getScale());
            collider->setOrientation(ownerObj->getTransform().getRotation());

            // Removed verbose position/velocity logging to reduce noise. Collision logs are produced in collision checks.
        }
    };
}

#endif
