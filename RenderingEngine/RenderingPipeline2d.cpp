#include "RenderingPipeline2d.h"
#include "RenderCommand.h"
#include "Sprite.h"
#include "TileMap.h"

namespace GUESS::rendering::twod {
    void RenderingPipeline2D::submitSprite(const Sprite& sprite, Material& material) {
        RenderCommand cmd;
        cmd.geometry.sprite = &sprite;
        cmd.material = &material;
        cmd.is2D = true;
        renderQueue.submit(cmd, static_cast<int>(sprite.getPosition().y));
    }

    void RenderingPipeline2D::submitTileMap(const TileMap& tilemap) {
        for (const auto& tileRow : tilemap.getTiles()) {
            for (const auto& tile : tileRow) {
                if (tile.isVisible) {
                    RenderCommand cmd;
                    cmd.geometry.sprite = &tile.sprite;
                    cmd.material = &tile.material;
                    cmd.is2D = true;
                    renderQueue.submit(cmd, 0);
                }
            }
        }
    }
    void RenderingPipeline2D::render(GUESS::rendering::Camera& camera, sf::RenderTarget& target)
    {
        renderQueue.execute(camera, target);

        postProcessor.process();
    }
}