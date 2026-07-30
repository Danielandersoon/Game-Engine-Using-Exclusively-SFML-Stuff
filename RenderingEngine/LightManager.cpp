#include "./LightManager.h"
#include <algorithm>
#include "../Logger.h"

namespace GUESS::rendering::threed {
    void LightManager::cullLights(const GUESS::core::math::Vector3f& cameraPosition) {
        visibleLights.clear();

        for (const auto& light : lights) {
            // Calculate distance between camera and light
            auto lightPos = light.getPosition();
            auto dx = lightPos.x - cameraPosition.x;
            auto dy = lightPos.y - cameraPosition.y;
            auto dz = lightPos.z - cameraPosition.z;
            float distSquared = dx * dx + dy * dy + dz * dz;

            // Check if light is within cull distance
            float cullDistanceSquared = cullDistance * cullDistance;
            if (distSquared <= cullDistanceSquared) {
                visibleLights.push_back(light);
            }
        }
    }

    void LightManager::updateLightUniforms(Shader& shader) {
        const size_t maxLights = 8;
        const size_t count = std::min(visibleLights.size(), maxLights);

        for (size_t i = 0; i < count; i++) {
            shader.setUniform("lightPositions[" + std::to_string(i) + "]", visibleLights[i].getPosition());
            shader.setUniform("lightColors[" + std::to_string(i) + "]", visibleLights[i].getColor());
            shader.setUniform("lightIntensities[" + std::to_string(i) + "]", visibleLights[i].getIntensity());
        }

        shader.setUniform("numLights", static_cast<int>(count));
    }
}
