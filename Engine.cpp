#include "./Engine.h"
#include "./MeshRendererComponent.h"
#include "./RigidbodyComponent.h"
#include "./Rigidbody2DComponent.h"


namespace GUESS::core {

    Engine::Engine(int instanceID) : m_instanceID(instanceID) {
        // Initialize managers
        m_sceneManager.Initialize();

        // Initialize physics systems
        m_physicsManager.initializeDefaultMaterials();

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
        m_sceneManager.getCurrentScene().setInputSystem(&m_inputSystem);
        m_sceneManager.Update();
        m_renderingPipeline.clear();

        Scene& currentScene = m_sceneManager.getCurrentScene();
        const auto& camera = currentScene.getMainCamera();

        // Process physics components
        for (const auto& [guid, gameObject] : currentScene.GetGameObjects()) {
            // Handle 3D physics
            if (auto* rb3d = gameObject->getComponent<GUESS::core::RigidbodyComponent>()) {
                if (auto* collider = rb3d->getRigidbody()->getCollider()) {
                    collider->setPosition(gameObject->getTransform().getPosition());
                    collider->setRotation(gameObject->getTransform().getRotation().toEuler().y);
                }
            }

            // Handle 2D physics
            if (auto* rb2d = gameObject->getComponent<GUESS::core::Rigidbody2DComponent>()) {
                gameObject->getTransform().setPosition(
                    GUESS::core::math::Vector3f(rb2d->getRigidbody()->getPosition().x, rb2d->getRigidbody()->getPosition().y, 0)
                );
                gameObject->getTransform().setRotation(
                    GUESS::core::math::Vector3f(0, 0, rb2d->getRigidbody()->getRotation())
                );
            }

            // Handle rendering
            if (auto* meshRenderer = gameObject->getComponent<MeshRendererComponent>()) {
                const auto& mesh = meshRenderer->getMesh();
                const auto& material = meshRenderer->getMaterial();
                const auto& worldMatrix = gameObject->getTransform().toMatrix();

                GUESS::rendering::RenderCommand cmd{ mesh->get(), material.get(), true};
                m_renderingPipeline.submitGeometry(cmd);
            }
        }
    }



    void Engine::fixedUpdate() {
        m_physicsWorld.step(FIXED_TIME_STEP);
    }

    void Engine::lateUpdate() {
        // Watch this space
    }
}
