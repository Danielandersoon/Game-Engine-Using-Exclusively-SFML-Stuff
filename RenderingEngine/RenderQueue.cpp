#include "./RenderQueue.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>

namespace GUESS::rendering {
    void RenderQueue::submit(const RenderCommand& cmd, int priority) {
        queues[priority].push_back(cmd);
    }

    void RenderQueue::execute(const Camera& camera, sf::RenderTarget& target) {
        for (auto& [priority, commands] : queues) {
            for (auto& cmd : commands) {
                if (cmd.geometry.mesh) {
                    float distance = (camera.getPosition() - cmd.geometry.mesh->getPosition()).magnitude();

                    // Select appropriate LOD level
                    GUESS::rendering::threed::Mesh* lodMesh = selectLOD(cmd.geometry.mesh, distance);

                    // Only render if mesh is in camera frustum
                    if (camera.isInFrustum(lodMesh->getBoundingBox(), lodMesh->getTransform())) {
                        if (cmd.material) {
                            const_cast<Material*>(cmd.material)->bind();

                            sf::RenderStates states;
                            states.shader = cmd.material->getShader().getNativeShader();
                            if (const sf::VertexBuffer* vb = lodMesh->getVertexArray()) {
                                target.draw(*vb, states);
                            }
                        }
                    }
                }
            }
        }
    }

    void RenderQueue::addLODLevel(GUESS::rendering::threed::Mesh* baseMesh, float distance, std::shared_ptr<GUESS::rendering::threed::Mesh> lodMesh) {
        LODLevel level;
        level.distance = distance;
        level.mesh = lodMesh;
        lodLevels[baseMesh].push_back(level);

        // Sort LOD levels by distance
        std::sort(lodLevels[baseMesh].begin(), lodLevels[baseMesh].end(),
            [](const LODLevel& a, const LODLevel& b) {
            return a.distance < b.distance;
        });
    }

    GUESS::rendering::threed::Mesh* RenderQueue::selectLOD(GUESS::rendering::threed::Mesh* mesh, float distance) {
        auto it = lodLevels.find(mesh);
        if (it == lodLevels.end()) {
            return mesh; // No LOD levels defined, return original mesh
        }

        // Find appropriate LOD level based on distance
        for (const auto& level : it->second) {
            if (distance > level.distance) {
                return level.mesh.get();
            }
        }

        return mesh; // Use original mesh if distance is closer than LOD thresholds
    }



}

