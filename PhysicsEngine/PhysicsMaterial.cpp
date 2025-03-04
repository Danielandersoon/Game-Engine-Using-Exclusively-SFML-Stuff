#include "./PhysicsMaterial.h"
#include "../Logger.h"

namespace GUESS::physics {

    void PhysicsMaterial::setStaticFriction(float value) {
        if (value >= 0.0f && value <= 1.0f) {
            staticFriction = value;
        }
        else {
            GUESS::core::Logger::log(GUESS::core::Logger::WARNING,
                "Static friction value must be between 0 and 1");
        }
    }

    void PhysicsMaterial::setDynamicFriction(float value) {
        if (value >= 0.0f && value <= 1.0f) {
            dynamicFriction = value;
        }
        else {
            GUESS::core::Logger::log(GUESS::core::Logger::WARNING,
                "Dynamic friction value must be between 0 and 1");
        }
    }

    void PhysicsMaterial::setRestitution(float value) {
        if (value >= 0.0f && value <= 1.0f) {
            restitution = value;
        }
        else {
            GUESS::core::Logger::log(GUESS::core::Logger::WARNING,
                "Restitution value must be between 0 and 1");
        }
    }

    void PhysicsMaterial::setDensity(float value) {
        if (value > 0.0f) {
            density = value;
        }
        else {
            GUESS::core::Logger::log(GUESS::core::Logger::WARNING,
                "Density value must be greater than 0");
        }
    }

    float PhysicsMaterial::combineFriction(const PhysicsMaterial& a, const PhysicsMaterial& b) {
        return GUESS::core::math::sqrt(a.getDynamicFriction() * b.getDynamicFriction());
    }

    float PhysicsMaterial::combineRestitution(const PhysicsMaterial& a, const PhysicsMaterial& b) {
        return std::min(a.getRestitution(), b.getRestitution());
    }
}
