#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H
#include <vector>
#include "./Shader.h"
#include "./Light.h"

namespace GUESS::rendering::threed
{
    class LightManager {
    private:
        std::vector<Light> lights;
        std::vector<Light> visibleLights;
        // Increase default cull distance so world-scale lights are not culled too aggressively
        float cullDistance = 2000.0f;
    public:
        void addLight(const Light& light) { lights.push_back(light); }
        void clearLights() { lights.clear(); visibleLights.clear(); }
        void updateLightUniforms(Shader& shader);
        void cullLights(const GUESS::core::math::Vector3f& cameraPosition);
        void setCullDistance(float distance) { cullDistance = distance; }
        const std::vector<Light>& getVisibleLights() const { return visibleLights; }
    };

}

#endif