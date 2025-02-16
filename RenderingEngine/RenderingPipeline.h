#ifndef RENDERING_PIPELINE_H
#define RENDERING_PIPELINE_H
#include "Mesh.h"
#include "Camera.h"
#include "Material.h"
#include "GBuffer.h"
#include "PostProcessor.h"
#include "shadowMapper.h"
#include "RenderQueue.h"

namespace GUESS::rendering {
    class RenderingPipeline {
    private:
        GBuffer gBuffer;
        PostProcessor postProcessor;
        GUESS::rendering::threed::ShadowMapper shadowMap;
        RenderQueue renderQueue;
        
    public:
        void initialize(int width, int height);
        void render(const Camera& camera, sf::RenderWindow& renderWindow);
        void submitGeometry(const RenderCommand& cmd);
        void clear() { renderQueue.clear(); }
    };
}

#endif