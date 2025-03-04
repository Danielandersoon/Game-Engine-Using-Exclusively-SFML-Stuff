#include "./RenderingPipeline2d.h"
#include "./RenderCommand.h"
#include "./Sprite.h"
#include "./TileMap.h"

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
                    cmd.material = tile.material.get();
                    cmd.is2D = true;
                    renderQueue.submit(cmd, 0);
                }
            }
        }
    }
    struct SpriteBatch {
        std::vector<sf::Vertex> vertices;
        const sf::Texture* texture;
        Material* material;
    };

    std::map<const sf::Texture*, SpriteBatch> batches;

    void RenderingPipeline2D::render(Camera& camera, sf::RenderTarget& target) {
        // Render all batches
        for (auto& [texture, batch] : batches) {
            if (batch.vertices.empty()) continue;

            batch.material->bind();

            sf::RenderStates states;
            states.shader = batch.material->getShader().getNativeShader();
            states.texture = batch.texture;

            target.draw(batch.vertices.data(), batch.vertices.size(), sf::Quads, states);
        }

        // Clear batches for next frame
        batches.clear();

        // Apply post-processing
        postProcessor.process();
    }

}