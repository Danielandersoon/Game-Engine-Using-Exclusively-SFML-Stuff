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
        float cullDistance = 100.0f;
    public:
        void addLight(const Light& light) { lights.push_back(light); }
        void updateLightUniforms(Shader& shader);
        void cullLights(const GUESS::core::math::Vector3f& cameraPosition);
        void setCullDistance(float distance) { cullDistance = distance; }
        const std::vector<Light>& getVisibleLights() const { return visibleLights; }
    };

} 

#endif