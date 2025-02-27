#ifndef PHYSICS_MATERIAL_H
#define PHYSICS_MATERIAL_H

#include <string>
#include "../GMath.h"

namespace GUESS::physics {
    class PhysicsMaterial {
    private:
        float staticFriction;
        float dynamicFriction;
        float restitution;
        float density;
        std::string name;

    public:
        PhysicsMaterial(const std::string& name = "Default", float staticFriction = 0.5f, float dynamicFriction = 0.3f, float restitution = 0.5f, float density = 1.0f)
            : name(name), staticFriction(staticFriction), dynamicFriction(dynamicFriction), restitution(restitution), density(density) {};

        // Getters
        float getStaticFriction() const { return staticFriction; }
        float getDynamicFriction() const { return dynamicFriction; }
        float getRestitution() const { return restitution; }
        float getDensity() const { return density; }
        const std::string& getName() const { return name; }

        // Setters
        void setStaticFriction(float value) { staticFriction = value; }
        void setDynamicFriction(float value) { dynamicFriction = value; }
        void setRestitution(float value) { restitution = value; }
        void setDensity(float value) { density = value; }
    
        // Combine friction values between two materials
        static float combineFriction(const PhysicsMaterial& a, const PhysicsMaterial& b) {
            return GUESS::core::math::sqrt(a.getDynamicFriction() * b.getDynamicFriction());
        }

        // Combine restitution values between two materials
        static float combineRestitution(const PhysicsMaterial& a, const PhysicsMaterial& b) {
            return std::min(a.getRestitution(), b.getRestitution());
        }
    };
}

#endif
