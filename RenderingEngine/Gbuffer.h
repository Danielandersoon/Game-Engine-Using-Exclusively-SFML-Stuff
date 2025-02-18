#ifndef GBUFFER_H
#define GBUFFER_H
#include <SFML/Graphics/RenderTexture.hpp>
#include "Shader.h"

namespace GUESS::rendering {
    class GBuffer {
    private:
        sf::RenderTexture positionBuffer;
        sf::RenderTexture normalBuffer;
        sf::RenderTexture albedoBuffer;
        sf::RenderTexture materialBuffer;
        sf::RenderTexture depthBuffer;
        GUESS::rendering::Shader geometryPass;
        GUESS::rendering::Shader lightingPass;

    public:
        void create(unsigned int width, unsigned int height);
        void bindForGeometryPass();
        void bindForLightingPass();
        void bindForDeferredPass();
        void clear();
        GUESS::rendering::Shader* getGeometryPass() { return &geometryPass; }
        const sf::Texture& getPositionTexture() const { return positionBuffer.getTexture(); }
        const sf::Texture& getNormalTexture() const { return normalBuffer.getTexture(); }
        const sf::Texture& getAlbedoTexture() const { return albedoBuffer.getTexture(); }
        const sf::Texture& getMaterialTexture() const { return materialBuffer.getTexture(); }
    };
}
#endif
