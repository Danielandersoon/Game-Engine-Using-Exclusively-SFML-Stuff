#ifndef RENDERING_PIPELINE_2D_H
#define RENDERING_PIPELINE_2D_H

#include "./RenderQueue.h"
#include "./PostProcessor.h"
#include "./Material.h"

namespace GUESS::rendering::twod {

    //Forward definitions otherwise it shits the bed
    class Sprite;
    class TileMap;

    class RenderingPipeline2D {
    private:
        GUESS::rendering::RenderQueue renderQueue;
        GUESS::rendering::PostProcessor postProcessor;

    public:
        void submitSprite(const Sprite& sprite, GUESS::rendering::Material& material);
        void submitTileMap(const TileMap& tilemap);
        void render(GUESS::rendering::Camera& camera, sf::RenderTarget& target);
    };
}

#endif
