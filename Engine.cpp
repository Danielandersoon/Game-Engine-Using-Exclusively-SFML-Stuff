#include "Engine.h"

namespace GUESS::core {

    Engine::Engine(int instanceID) : m_instanceID(instanceID) {
        // Initialize managers
        m_sceneManager.Initialize();
        m_windowManager.Initialize();
        start();
    }

    unsigned int Engine::getInstanceID() {
        return m_instanceID;
    }

    void Engine::start() {
        // Log game start
        Logger::log(Logger::INFO, "Game initialized");

        m_isRunning = true;

        while (m_isRunning) {
            // Update timing
            m_timingSystem.TimingUpdate();
            float deltaTime = m_timingSystem.DeltaTime();

            // Accumulate time for fixed updates
            m_accumulatedTime += deltaTime;

            // Handle fixed time step updates
            while (m_accumulatedTime >= FIXED_TIME_STEP) {
                fixedUpdate();
                m_accumulatedTime -= FIXED_TIME_STEP;
            }

            // Regular update cycle
            update();

            // Late update cycle
            lateUpdate();
        }
    }

    void Engine::stop() {
        m_isRunning = false;
    }

    void Engine::update() {
        m_inputSystem.Update();
        m_sceneManager.Update();
        m_renderingPipeline.clear();

        Scene& currentScene = m_sceneManager.getCurrentScene();
        const auto& camera = currentScene.getMainCamera();

        // Spatial partitioning grid
        std::vector<GameObject*> visibleObjects;

        for (const auto& gameObject : currentScene.GetGameObjects()) {
            if (auto* meshRenderer = gameObject->getComponent<GUESS::rendering::threed::MeshRendererComponenet>()) {
                const auto& mesh = meshRenderer->getMesh();
                const auto& worldMatrix = gameObject->getTransform().toMatrix();

                // Frustum culling check
                if (camera.isInFrustum(mesh->getBoundingBox(), worldMatrix)) {
                    GUESS::rendering::RenderCommand cmd{
                        meshRenderer->getMesh(),
                        meshRenderer->getMaterial(),
                        worldMatrix,
                    };
                    m_renderingPipeline.submitGeometry(cmd);
                }
            }
        }
    }

    void Engine::fixedUpdate() {
        // Watch this space
    }

    void Engine::lateUpdate() {
        // Watch this space
    }
}
