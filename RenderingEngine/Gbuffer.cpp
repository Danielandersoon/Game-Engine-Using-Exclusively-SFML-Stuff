#include "./Gbuffer.h"

namespace GUESS::rendering {
    void GBuffer::create(unsigned int width, unsigned int height) {
        positionBuffer.create(width, height);
        normalBuffer.create(width, height);
        albedoBuffer.create(width, height);
        materialBuffer.create(width, height);
        depthBuffer.create(width, height);

        geometryPass.loadFromFile("shaders/geometry_pass.vert", "shaders/geometry_pass.frag");
        lightingPass.loadFromFile("shaders/lighting_pass.vert", "shaders/lighting_pass.frag");

        // Set up render targets
        positionBuffer.setSmooth(false);
        normalBuffer.setSmooth(false);
        albedoBuffer.setSmooth(false);
        materialBuffer.setSmooth(false);
        depthBuffer.setSmooth(false);
    }

    void GBuffer::bindForGeometryPass() {
        positionBuffer.setActive(true);
        normalBuffer.setActive(true);
        albedoBuffer.setActive(true);
        materialBuffer.setActive(true);
        depthBuffer.setActive(true);

        geometryPass.bind();
    }

    void GBuffer::bindForLightingPass() {
        lightingPass.bind();
        lightingPass.setUniform("gPosition", positionBuffer.getTexture());
        lightingPass.setUniform("gNormal", normalBuffer.getTexture());
        lightingPass.setUniform("gAlbedo", albedoBuffer.getTexture());
        lightingPass.setUniform("gMaterial", materialBuffer.getTexture());
        lightingPass.setUniform("gDepth", depthBuffer.getTexture());
    }

    void GBuffer::bindForDeferredPass() {
        lightingPass.bind();
        // Bind all textures for final composition
        int textureUnit = 0;
        lightingPass.setUniform("gFinalComposite", textureUnit++);
    }

    void GBuffer::clear() {
        positionBuffer.clear(sf::Color::Black);
        normalBuffer.clear(sf::Color::Black);
        albedoBuffer.clear(sf::Color::Black);
        materialBuffer.clear(sf::Color::Black);
        depthBuffer.clear(sf::Color::White);
    }
}
