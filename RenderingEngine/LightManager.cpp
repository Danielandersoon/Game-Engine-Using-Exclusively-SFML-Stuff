#include "./LightManager.h"

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
        // Update only visible lights
        for (size_t i = 0; i < visibleLights.size(); i++) {
            shader.setUniform("lights[" + std::to_string(i) + "].position", visibleLights[i].getPosition());
            shader.setUniform("lights[" + std::to_string(i) + "].color", visibleLights[i].getColor());
            shader.setUniform("lights[" + std::to_string(i) + "].intensity", visibleLights[i].getIntensity());
            shader.setUniform("lights[" + std::to_string(i) + "].shadowMap", visibleLights[i].getShadowMap());
            shader.setUniform("lights[" + std::to_string(i) + "].lightSpaceMatrix", visibleLights[i].getLightSpaceMatrix());
        }
        // Set the number of active lights
        shader.setUniform("numLights", static_cast<int>(visibleLights.size()));
    }
}
