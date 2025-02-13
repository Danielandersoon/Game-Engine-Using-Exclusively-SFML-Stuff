#include "RenderManager.h"

namespace GUESS::rendering {
    RenderManager::RenderManager() : m_targetWindow(nullptr), m_isVsyncEnabled(true) {
        Initialize();
    }

    bool RenderManager::Initialize() {
        if (!m_initialized) {
            m_initialized = true;
            m_running = true;
            m_mainCamera = std::make_unique<Camera>(800.0f, 600.0f);
            return true;
        }
        return true;
    }

    bool RenderManager::Shutdown() {
        if (m_initialized) {
            ClearQueue();
            m_mainCamera.reset();
            m_targetWindow = nullptr;
            m_initialized = false;
            m_running = false;
            return true;
        }
        return false;
    }

    bool RenderManager::Update() {
        if (m_initialized && m_running && m_targetWindow) {
            Draw();
            return true;
        }
        return false;
    }

    void RenderManager::SetTargetWindow(sf::RenderWindow* window) {
        m_targetWindow = window;
        if (m_targetWindow) {
            m_targetWindow->setVerticalSyncEnabled(m_isVsyncEnabled);
        }
    }

    void RenderManager::SetMainCamera(std::unique_ptr<Camera> camera) {
        m_mainCamera = std::move(camera);
    }

    void RenderManager::Submit(sf::Drawable* drawable) {
        if (drawable) {
            m_renderQueue.push_back(drawable);
        }
    }

    void RenderManager::ClearQueue() {
        m_renderQueue.clear();
    }

    void RenderManager::EnableVsync(bool enable) {
        m_isVsyncEnabled = enable;
        if (m_targetWindow) {
            m_targetWindow->setVerticalSyncEnabled(enable);
        }
    }

    void RenderManager::Draw() {
        if (!m_targetWindow || !m_mainCamera) return;

        m_targetWindow->clear();
        m_targetWindow->setView(m_mainCamera->getView());

        // Draw 3D meshes first
        for (auto* mesh : m_meshRenderQueue) {
            if (mesh && m_mainCamera->isInView(mesh->getBoundingBox())) {
                DrawMesh(*mesh, mesh->getTransform());
            }
        }

        // Draw 2D elements on top
        for (auto* drawable : m_renderQueue) {
            m_targetWindow->draw(*drawable);
        }

        m_targetWindow->display();
        ClearQueue();
        m_meshRenderQueue.clear();
    }

    void RenderManager::DrawMesh(const GUESS::rendering::threed::Mesh& mesh, const GUESS::core::math::Matrix4x4& transform) {
        // Convert GUESS math types to SFML types
        sf::VertexArray vertices(sf::Triangles);

        const auto& meshVertices = mesh.getVertices();
        const auto& indices = mesh.getIndices();

        for (size_t i = 0; i < indices.size(); i += 3) {
            for (int j = 0; j < 3; j++) {
                const auto& vertex = meshVertices[indices[i + j]];

                // Project 3D position to 2D
                GUESS::core::math::Vector3f worldPos = transform * vertex.position;
                // Use the camera's transform to get a 2D position.
                sf::Vector2f viewPos = m_mainCamera->getView().getTransform().transformPoint(sf::Vector2f(worldPos.x, worldPos.y));

                sf::Vertex sfmlVertex;
                sfmlVertex.position = viewPos;
                sfmlVertex.color = vertex.color;
                sfmlVertex.texCoords = sf::Vector2f(vertex.texCoords.x, vertex.texCoords.y);

                vertices.append(sfmlVertex);
            }
        }

        m_targetWindow->draw(vertices, m_currentRenderState);
    }

    void RenderManager::SubmitMesh(GUESS::rendering::threed::Mesh* mesh) {
        if (mesh) {
            m_meshRenderQueue.push_back(mesh);
        }
    }
}