#include "./BoxCollider.h"

namespace GUESS::physics {
    BoxCollider::BoxCollider(const GUESS::core::math::Vector3f& dimensions) {
        this->dimensions = dimensions;
        position = GUESS::core::math::Vector3f(0.0f, 0.0f, 0.0f);
        center = position;
        scale = GUESS::core::math::Vector3f(1.0f, 1.0f, 1.0f);
        rotation = 0.0f;
        isTrigger = false;
    }

    bool BoxCollider::checkCollision(const Collider<GUESS::core::math::Vector3f>& other) {
        // Get the centers and dimensions of both boxes
        GUESS::core::math::Vector3f centerA = getCenter();
        GUESS::core::math::Vector3f centerB = other.getCenter();

        GUESS::core::math::Vector3f dimA = getDimensions().dot(scale);
        GUESS::core::math::Vector3f dimB = other.getDimensions().dot(other.getScale());

        // Calculate the minimum and maximum points for both boxes
        GUESS::core::math::Vector3f minA = centerA - (dimA * 0.5f);
        GUESS::core::math::Vector3f maxA = centerA + (dimA * 0.5f);
        GUESS::core::math::Vector3f minB = centerB - (dimB * 0.5f);
        GUESS::core::math::Vector3f maxB = centerB + (dimB * 0.5f);

        // Check for overlap in all three axes (AABB collision check)
        bool overlapX = (minA.x <= maxB.x) && (maxA.x >= minB.x);
        bool overlapY = (minA.y <= maxB.y) && (maxA.y >= minB.y);
        bool overlapZ = (minA.z <= maxB.z) && (maxA.z >= minB.z);

        // Collision occurs only if there is overlap in all axes
        return overlapX && overlapY && overlapZ;
    }


    void BoxCollider::onCollisionEnter(Collider<GUESS::core::math::Vector3f>& other) {
        if (!isTrigger && !other.getTrigger()) {
            resolveCollision(other);
        }
    }

    void BoxCollider::onCollisionStay(Collider<GUESS::core::math::Vector3f>& other) {
        if (!isTrigger && !other.getTrigger()) {
            resolveCollision(other);
        }
    }

    void BoxCollider::onCollisionExit(Collider<GUESS::core::math::Vector3f>& other) {
        // Im sure something should go here
     }

    void BoxCollider::resolveCollision(Collider<GUESS::core::math::Vector3f>& other) {
        auto* thisRB = getAttachedRigidbody();
        auto* otherRB = other.getAttachedRigidbody();

        if (!thisRB || !otherRB) return;

        // Calculate collision normal
        GUESS::core::math::Vector3f normal = (other.getCenter() - getCenter()).normalized();

        // Calculate relative velocity
        GUESS::core::math::Vector3f relativeVel = otherRB->getVelocity() - thisRB->getVelocity();

        // Calculate restitution (use the lowest of the two objects)
        float restitution = std::min(thisRB->getRestitution(), otherRB->getRestitution());

        // Calculate impulse scalar
        float j = -(1.0f + restitution) * relativeVel.dot(normal);
        j /= (1.0f / thisRB->getMass()) + (1.0f / otherRB->getMass());

        // Apply impulse
        GUESS::core::math::Vector3f impulse = normal * j;
        thisRB->addForce(impulse * -1);
        otherRB->addForce(impulse);
    }
}
