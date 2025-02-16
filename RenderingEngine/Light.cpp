#include "Light.h"

namespace GUESS::rendering::threed {

    Light::Light(LightType type) : type(type), intensity(1.0f), colour(1.0f, 1.0f, 1.0f) {
        position = GUESS::core::math::Vector3f(0.0f, 0.0f, 0.0f);
    }

    void Light::setForward(const GUESS::core::math::Vector3f& forward) {
        GUESS::core::math::Vector3f currentForward(0, 0, 1);
        GUESS::core::math::Vector3f rotationAxis = currentForward.cross(forward).normalized();
        float angle = GUESS::core::math::arccos(currentForward.dot(forward));

        // Create quaternion from axis-angle
        float sinHalfAngle = GUESS::core::math::sin(angle / 2.0f);
        rotation.x = rotationAxis.x * sinHalfAngle;
        rotation.y = rotationAxis.y * sinHalfAngle;
        rotation.z = rotationAxis.z * sinHalfAngle;
        rotation.w = GUESS::core::math::cos(angle / 2.0f);
    }

    void Light::setShadowMap(const sf::Texture& shadowMap) {
        m_shadowMap = shadowMap;
    }

    void Light::setLightSpaceMatrix(const GUESS::core::math::Matrix4x4& matrix) {
        m_lightSpaceMatrix = matrix;
    }

}
