#include "Gbuffer.h"

namespace GUESS::rendering {
    void GBuffer::create(unsigned int width, unsigned int height) {
        positionBuffer.create(width, height);
        normalBuffer.create(width, height);
        albedoBuffer.create(width, height);
        materialBuffer.create(width, height);
        
        geometryPass.loadFromFile("shaders/geometry_pass.vert", "shaders/geometry_pass.frag");
    }
    
    void GBuffer::bindForGeometryPass() {
        positionBuffer.setActive(true);
        normalBuffer.setActive(true);
        albedoBuffer.setActive(true);
        materialBuffer.setActive(true);
        geometryPass.bind();
    }
    
    void GBuffer::bindForLightingPass() {
        geometryPass.setUniform("gPosition", positionBuffer.getTexture());
        geometryPass.setUniform("gNormal", normalBuffer.getTexture());
        geometryPass.setUniform("gAlbedo", albedoBuffer.getTexture());
        geometryPass.setUniform("gMaterial", materialBuffer.getTexture());
    }
    
}