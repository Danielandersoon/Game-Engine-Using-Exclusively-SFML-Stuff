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
        GUESS::rendering::Shader geometryPass;
        
    public:
        void create(unsigned int width, unsigned int height);
        void bindForGeometryPass();
        void bindForLightingPass();
        GUESS::rendering::Shader* getGeometryPass() { return &geometryPass; }
    };
}

#endif