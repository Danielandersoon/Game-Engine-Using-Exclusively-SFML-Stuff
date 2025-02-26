#ifndef RIGIDBODY_COMPONENT_H
#define RIGIDBODY_COMPONENT_H

#include "Component.h"
#include "PhysicsEngine/RigidBody.h"
#include "PhysicsEngine/Collider.h"

namespace GUESS::core {
    class RigidbodyComponent : public Component {
    private:
        std::unique_ptr<GUESS::physics::Rigidbody3D> rigidbody;
        
    public:
        void init() override {
            setName("rigidbody component");
            rigidbody = std::make_unique<GUESS::physics::Rigidbody3D>();
        }

        void update() {
            // Update transform from physics simulation
            const auto& pos = rigidbody->getPosition();
            getOwner()->getTransform().setPosition(pos);
        }

        void setMass(float mass) { rigidbody->setMass(mass); }
        void setFriction(float friction) { rigidbody->setFriction(friction); }
        void setRestitution(float restitution) { rigidbody->setRestitution(restitution); }
        
        void addForce(const GUESS::core::math::Vector3f& force) {
            rigidbody->addForce(force);
        }

        GUESS::physics::Rigidbody3D* getRigidbody() { return rigidbody.get(); }
    };
}
#endif
