#ifndef SHADOW_MAPPER_H
#define SHADOW_MAPPER_H
#include "Shader.h"
#include <SFML/Graphics/RenderTexture.hpp>

namespace GUESS::rendering::threed {
    class ShadowMapper {
    private:
        sf::RenderTexture shadowMapTexture;
        const unsigned int SHADOW_WIDTH = 1024;
        const unsigned int SHADOW_HEIGHT = 1024;
        GUESS::core::math::Matrix4x4 lightSpaceMatrix;

    public:
        void initialize();
        void beginShadowPass();
        void endShadowPass();
        void bindShadowMap(Shader& shader, int textureUnit = 0);
        const GUESS::core::math::Matrix4x4& getLightSpaceMatrix() const;
    };
}
#endif
