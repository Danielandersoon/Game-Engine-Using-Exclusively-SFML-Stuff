#include "RenderingPipeline.h"

namespace GUESS::rendering {
    void RenderingPipeline::initialize(int width, int height) {
        gBuffer.create(width, height);
        postProcessor.initialize(width, height);
        shadowMap.initialize();
    }
    
    void RenderingPipeline::submitGeometry(const RenderCommand& cmd) {
        renderQueue.submit(cmd, 0);
    }
    
    void RenderingPipeline::render(const Camera& camera, sf::RenderWindow& renderWindow) {
        // Shadow pass
        shadowMap.beginShadowPass();
        renderQueue.execute(camera, renderWindow);
        shadowMap.endShadowPass();

        // Geometry pass
        gBuffer.bindForGeometryPass();
        renderQueue.execute(camera, renderWindow);

        // Lighting pass
        gBuffer.bindForLightingPass();
        renderQueue.execute(camera, renderWindow);

        // Post processing
        postProcessor.process();
    }
    
}