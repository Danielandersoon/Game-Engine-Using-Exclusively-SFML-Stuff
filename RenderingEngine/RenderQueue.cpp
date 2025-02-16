#include "RenderQueue.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>

namespace GUESS::rendering {
    void RenderQueue::submit(const RenderCommand& cmd, int priority) {
        queues[priority].push_back(cmd);
    }

    void RenderQueue::execute(const Camera& camera, sf::RenderTarget& renderTarget) {
        // Sort by material to minimize state changes
        for (auto& [priority, commands] : queues) {
            std::sort(commands.begin(), commands.end(),
                [](const RenderCommand& a, const RenderCommand& b) {
                return a.material < b.material;
            });

            Material* currentMaterial = nullptr;
            std::vector<sf::Vertex> batchedVertices;

            for (const auto& cmd : commands) {
                // New material - flush existing batch
                if (currentMaterial != cmd.material) {
                    if (!batchedVertices.empty()) {
                        renderTarget.draw(batchedVertices.data(), batchedVertices.size(),
                            sf::Triangles, sf::RenderStates(currentMaterial->getShader().getNativeShader()));
                        batchedVertices.clear();
                    }
                    currentMaterial = cmd.material;
                    currentMaterial->bind();
                }

                // Add to current batch
                const auto& mesh = cmd.mesh;
                const auto& vertices = mesh->getVertices();
                const auto& indices = mesh->getIndices();

                // Transform and add vertices to batch
                for (unsigned int index : indices) {
                    const auto& vertex = vertices[index];
                    sf::Vertex transformedVertex;
                    auto worldPos = cmd.transform * vertex.position;
                    transformedVertex.position = sf::Vector2f(worldPos.x, worldPos.y);
                    transformedVertex.color = vertex.color;
                    transformedVertex.texCoords = sf::Vector2f(vertex.texCoords.x, vertex.texCoords.y);
                    batchedVertices.push_back(transformedVertex);
                }
            }

            // Flush final batch
            if (!batchedVertices.empty()) {
                renderTarget.draw(batchedVertices.data(), batchedVertices.size(),
                    sf::Triangles, sf::RenderStates(currentMaterial->getShader().getNativeShader()));
            }
        }
    }
}

