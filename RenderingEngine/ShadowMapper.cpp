#include "ShadowMapper.h"

namespace GUESS::rendering::threed {

    void ShadowMapper::initialize() {
        shadowMapTexture.create(SHADOW_WIDTH, SHADOW_HEIGHT);
        shadowMapTexture.setSmooth(true);
    }

    void ShadowMapper::beginShadowPass() {
        shadowMapTexture.clear(sf::Color::White);
    }

    void ShadowMapper::endShadowPass() {
        shadowMapTexture.display();
    }

    void ShadowMapper::bindShadowMap(Shader& shader, int textureUnit) {
        shader.setUniform("shadowMap", shadowMapTexture.getTexture());
        shader.setUniform("lightSpaceMatrix", lightSpaceMatrix);
    }

    const GUESS::core::math::Matrix4x4& ShadowMapper::getLightSpaceMatrix() const {
        return lightSpaceMatrix;
    }
}
