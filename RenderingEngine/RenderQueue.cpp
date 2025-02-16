#include "RenderQueue.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>

namespace GUESS::rendering {
    void RenderQueue::submit(const RenderCommand& cmd, int priority) {
        queues[priority].push_back(cmd);
    }

    void RenderQueue::execute(const Camera& camera, sf::RenderTarget& renderTarget) {
        for (const auto& [priority, commands] : queues) {
            for (const auto& cmd : commands) {
                cmd.material->bind();
                cmd.material->getShader().setUniform("modelMatrix", cmd.transform);
                cmd.material->getShader().setUniform("viewMatrix", camera.getViewMatrix());
                cmd.material->getShader().setUniform("projectionMatrix", camera.getProjectionMatrix());

                sf::VertexArray vertexArray(sf::Triangles);
                const auto& vertices = cmd.mesh->getVertices();
                const auto& indices = cmd.mesh->getIndices();

                if (!indices.empty()) {
                    for (unsigned int index : indices) {
                        const auto& vertex = vertices[index];
                        sf::Vertex sfmlVertex;
                        sfmlVertex.position = sf::Vector2f(vertex.position.x, vertex.position.y);
                        sfmlVertex.color = vertex.color;
                        sfmlVertex.texCoords = sf::Vector2f(vertex.texCoords.x, vertex.texCoords.y);
                        vertexArray.append(sfmlVertex);
                    }
                }

                sf::RenderStates states;
                states.shader = cmd.material->getShader().getNativeShader();
                renderTarget.draw(vertexArray, states);
            }
        }
    }
}

