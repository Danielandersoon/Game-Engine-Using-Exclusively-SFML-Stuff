#include "./BoxCollider.h"
#include "../Logger.h"
#include <algorithm>

namespace GUESS::physics {
    // Helper: Calculate AABB bounds that encompass a rotated box
    static void calculateRotatedAABB(const GUESS::core::math::Vector3f& center,
                                      const GUESS::core::math::Vector3f& halfExtents,
                                      const GUESS::core::math::Quaternion& orientation,
                                      GUESS::core::math::Vector3f& outMin,
                                      GUESS::core::math::Vector3f& outMax) {
        // Get the 8 corners of the box in local space
        GUESS::core::math::Vector3f corners[8] = {
            GUESS::core::math::Vector3f(-halfExtents.x, -halfExtents.y, -halfExtents.z),
            GUESS::core::math::Vector3f( halfExtents.x, -halfExtents.y, -halfExtents.z),
            GUESS::core::math::Vector3f(-halfExtents.x,  halfExtents.y, -halfExtents.z),
            GUESS::core::math::Vector3f( halfExtents.x,  halfExtents.y, -halfExtents.z),
            GUESS::core::math::Vector3f(-halfExtents.x, -halfExtents.y,  halfExtents.z),
            GUESS::core::math::Vector3f( halfExtents.x, -halfExtents.y,  halfExtents.z),
            GUESS::core::math::Vector3f(-halfExtents.x,  halfExtents.y,  halfExtents.z),
            GUESS::core::math::Vector3f( halfExtents.x,  halfExtents.y,  halfExtents.z)
        };

        // Rotate all corners and find min/max bounds
        outMin = GUESS::core::math::Vector3f(FLT_MAX, FLT_MAX, FLT_MAX);
        outMax = GUESS::core::math::Vector3f(-FLT_MAX, -FLT_MAX, -FLT_MAX);

        for (int i = 0; i < 8; i++) {
            // Rotate corner by orientation quaternion
            GUESS::core::math::Vector3f rotatedCorner = orientation * corners[i];
            // Translate to world space
            rotatedCorner = rotatedCorner + center;

            // Update bounds
            outMin.x = std::min(outMin.x, rotatedCorner.x);
            outMin.y = std::min(outMin.y, rotatedCorner.y);
            outMin.z = std::min(outMin.z, rotatedCorner.z);
            outMax.x = std::max(outMax.x, rotatedCorner.x);
            outMax.y = std::max(outMax.y, rotatedCorner.y);
            outMax.z = std::max(outMax.z, rotatedCorner.z);
        }
    }

    BoxCollider::BoxCollider(const GUESS::core::math::Vector3f& dimensions) {
        this->dimensions = dimensions;
        position = GUESS::core::math::Vector3f(0.0f, 0.0f, 0.0f);
        center = position;
        scale = GUESS::core::math::Vector3f(1.0f, 1.0f, 1.0f);
        rotation = 0.0f;
        isTrigger = false;
    }

    bool BoxCollider::checkCollision(const Collider<GUESS::core::math::Vector3f>& other) {
        // Get the centers and scaled dimensions of both boxes
        GUESS::core::math::Vector3f centerA = getCenter();
        GUESS::core::math::Vector3f centerB = other.getCenter();

        GUESS::core::math::Vector3f dimA = getDimensions() * scale; 
        GUESS::core::math::Vector3f dimB = other.getDimensions() * other.getScale();

        // Calculate AABB bounds considering rotation
        GUESS::core::math::Vector3f minA, maxA, minB, maxB;
        calculateRotatedAABB(centerA, dimA * 0.5f, getOrientation(), minA, maxA);
        calculateRotatedAABB(centerB, dimB * 0.5f, other.getOrientation(), minB, maxB);

        // Check for overlap in all three axes (AABB collision check)
        bool overlapX = (minA.x <= maxB.x) && (maxA.x >= minB.x);
        bool overlapY = (minA.y <= maxB.y) && (maxA.y >= minB.y);
        bool overlapZ = (minA.z <= maxB.z) && (maxA.z >= minB.z);

        bool colliding = overlapX && overlapY && overlapZ;
        if (colliding) {
            std::string msg = "AABB collision detected between centers A=(" + std::to_string(centerA.x) + "," + std::to_string(centerA.y) + "," + std::to_string(centerA.z) + ")";
            msg += " B=(" + std::to_string(centerB.x) + "," + std::to_string(centerB.y) + "," + std::to_string(centerB.z) + ")";
            GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, msg);
        }

        return colliding;
    }


    void BoxCollider::onCollisionEnter(Collider<GUESS::core::math::Vector3f>& other) {
        // Collision resolution is handled by PhysicsWorld, not here
        // This prevents double-resolution which causes jittering
    }

    void BoxCollider::onCollisionStay(Collider<GUESS::core::math::Vector3f>& other) {
        // Collision resolution is handled by PhysicsWorld, not here
        // This prevents double-resolution which causes jittering
    }

    void BoxCollider::onCollisionExit(Collider<GUESS::core::math::Vector3f>& other) {
        // Im sure something should go here
     }

    void BoxCollider::resolveCollision(Collider<GUESS::core::math::Vector3f>& other) {
        auto* thisRB = getAttachedRigidbody();
        auto* otherRB = other.getAttachedRigidbody();

        if (!thisRB || !otherRB) return;

        // Calculate collision normal (from this to other)
        GUESS::core::math::Vector3f normal = (other.getCenter() - getCenter());
        float nmag = normal.magnitude();
        if (nmag == 0.0f) return; // avoid divide by zero
        normal = normal * (1.0f / nmag);

        // Calculate relative velocity along normal
        GUESS::core::math::Vector3f relativeVel = otherRB->getVelocity() - thisRB->getVelocity();
        float velAlongNormal = relativeVel.dot(normal);

        std::string debugMsg = "[BoxCollider] velAlongNormal=" + std::to_string(velAlongNormal) + " normal=(" + std::to_string(normal.x) + "," + std::to_string(normal.y) + "," + std::to_string(normal.z) + ")";
        GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, debugMsg);

        // Do not resolve if velocities are separating
        if (velAlongNormal > 0.0f) {
            GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, "[BoxCollider] Early return: velocities already separating");
            return;
        }

        // Calculate restitution (use the lowest of the two objects)
        float restitution = std::min(thisRB->getRestitution(), otherRB->getRestitution());

        // Calculate impulse scalar (J)
        float invMassA = thisRB->getIsStatic() ? 0.0f : 1.0f / thisRB->getMass();
        float invMassB = otherRB->getIsStatic() ? 0.0f : 1.0f / otherRB->getMass();

        std::string massMsg = "[BoxCollider] invMassA=" + std::to_string(invMassA) + " invMassB=" + std::to_string(invMassB);
        GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, massMsg);

        if (invMassA + invMassB <= 0.0f) {
            GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, "[BoxCollider] Early return: both objects static");
            return;
        }

        float j = -(1.0f + restitution) * velAlongNormal;
        j /= (invMassA + invMassB);

        // Apply impulse directly to velocities
        GUESS::core::math::Vector3f impulse = normal * j;

        std::string impulseMsg = "[BoxCollider] restitution=" + std::to_string(restitution) + " j=" + std::to_string(j) + " impulse=(" + std::to_string(impulse.x) + "," + std::to_string(impulse.y) + "," + std::to_string(impulse.z) + ")";
        GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, impulseMsg);

        if (invMassA > 0.0f) {
            auto oldVelA = thisRB->getVelocity();
            auto newVelA = thisRB->getVelocity() - impulse * invMassA;
            thisRB->setVelocity(newVelA);
            std::string velMsgA = "[BoxCollider] ThisRB velocity changed from (" + std::to_string(oldVelA.x) + "," + std::to_string(oldVelA.y) + "," + std::to_string(oldVelA.z) + ") to (" + std::to_string(newVelA.x) + "," + std::to_string(newVelA.y) + "," + std::to_string(newVelA.z) + ")";
            GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, velMsgA);
        }
        if (invMassB > 0.0f) {
            auto oldVelB = otherRB->getVelocity();
            auto newVelB = otherRB->getVelocity() + impulse * invMassB;
            otherRB->setVelocity(newVelB);
            std::string velMsgB = "[BoxCollider] OtherRB velocity changed from (" + std::to_string(oldVelB.x) + "," + std::to_string(oldVelB.y) + "," + std::to_string(oldVelB.z) + ") to (" + std::to_string(newVelB.x) + "," + std::to_string(newVelB.y) + "," + std::to_string(newVelB.z) + ")";
            GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, velMsgB);
        }
    }
}
