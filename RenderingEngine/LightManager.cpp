#include "LightManager.h"

namespace GUESS::rendering::threed {
    void LightManager::updateLightUniforms(Shader& shader) {
        for (size_t i = 0; i < lights.size(); i++) {
            shader.setUniform("lights[" + std::to_string(i) + "].position", lights[i].getPosition());
            shader.setUniform("lights[" + std::to_string(i) + "].color", lights[i].getColor());
            shader.setUniform("lights[" + std::to_string(i) + "].intensity", lights[i].getIntensity());
            shader.setUniform("lights[" + std::to_string(i) + "].shadowMap", lights[i].getShadowMap());
            shader.setUniform("lights[" + std::to_string(i) + "].lightSpaceMatrix", lights[i].getLightSpaceMatrix());
        }
    }
}
