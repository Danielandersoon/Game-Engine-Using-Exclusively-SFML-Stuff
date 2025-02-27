#include "./CapsuleCollider.h"
#include <cmath>

namespace GUESS::physics {

    CapsuleCollider::CapsuleCollider(float radius, float height)
        : radius(radius), height(height) {
        dimensions = GUESS::core::math::Vector3f(radius * 2, height, radius * 2);
        center = GUESS::core::math::Vector3f(0, 0, 0);
    }

    bool CapsuleCollider::checkCollision(const Collider<GUESS::core::math::Vector3f>& other) {
        // Get the closest point on the capsule's center line to the other collider's center
        GUESS::core::math::Vector3f capsuleStart = position;
        GUESS::core::math::Vector3f capsuleEnd = position + GUESS::core::math::Vector3f(0, height, 0);
        GUESS::core::math::Vector3f otherCenter = other.getCenter();

        // Calculate vector from start to end of capsule
        GUESS::core::math::Vector3f capsuleDir = capsuleEnd - capsuleStart;
        float capsuleLength = std::sqrt(capsuleDir.x * capsuleDir.x +
            capsuleDir.y * capsuleDir.y +
            capsuleDir.z * capsuleDir.z);

        capsuleDir = capsuleDir / capsuleLength;

        // Calculate closest point on capsule line to other center
        GUESS::core::math::Vector3f toOther = otherCenter - capsuleStart;
        float projection = toOther.x * capsuleDir.x +
            toOther.y * capsuleDir.y +
            toOther.z * capsuleDir.z;

        projection = std::max(0.0f, std::min(capsuleLength, projection));

        GUESS::core::math::Vector3f closestPoint = capsuleStart + capsuleDir * projection;

        // Calculate distance between closest point and other center
        GUESS::core::math::Vector3f distanceVec = otherCenter - closestPoint;
        float distance = std::sqrt(distanceVec.x * distanceVec.x +
            distanceVec.y * distanceVec.y +
            distanceVec.z * distanceVec.z);

        // Check if distance is less than sum of radii
        return distance <= radius + other.getDimensions().x / 2;
    }
}
