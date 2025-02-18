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

        std::map<const GUESS::rendering::threed::Mesh*, std::vector<GUESS::core::math::Matrix4x4>> instancedMeshes;

        for (const auto& gameObject : currentScene.GetGameObjects()) {
            if (auto* meshRenderer = gameObject->getComponent<GUESS::rendering::threed::MeshRendererComponenet>()) {
                const auto& mesh = meshRenderer->getMesh();
                const auto& material = meshRenderer->getMaterial();  
                const auto& worldMatrix = gameObject->getTransform().toMatrix();

                instancedMeshes[&(*mesh)].push_back(worldMatrix);
                GUESS::rendering::RenderCommand cmd{ mesh->get(), material, true };
                m_renderingPipeline.submitGeometry(cmd);
            }
        }

        // For instanced rendering
        for (const auto& [mesh, transforms] : instancedMeshes) {
            if (transforms.size() > 1) {
                mesh->setInstanceTransforms(transforms);
                // Use the material from the renderer, not from mesh
                GUESS::rendering::RenderCommand cmd{ mesh, nullptr, true }; 
                m_renderingPipeline.submitGeometry(cmd);
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
