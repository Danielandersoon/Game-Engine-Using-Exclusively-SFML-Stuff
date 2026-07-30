#include "./Engine.h"

// Physics components
#include "./MeshRendererComponent.h"
#include "./RigidbodyComponent.h"
#include "./Rigidbody2DComponent.h"
#include "./PhysicsEngine/BoxColliderComponent.h"
#include "./PhysicsEngine/ThermodynamicBodyComponent.h"
#include "./CapsuleCollisionComponent.h"
#include "./FluidSimulationComponent.h"

// Render components
#include "./SpriteRenderComponent.h"
#include "./MeshRendererComponent.h"
#include "./CameraComponent.h"
#include "./LightComponents.h"



namespace GUESS::core {

    Engine::Engine(int instanceID) : m_instanceID(instanceID) {
        // Initialize managers
        m_sceneManager.Initialize();
        // Note: Caller should explicitly call start() when ready to begin game loop
    }

    unsigned int Engine::getInstanceID() {
        return m_instanceID;
    }

    void Engine::start() {
        // Log game start
        Logger::log(Logger::INFO, "Game initialized");

        try {
            Logger::log(Logger::INFO, "Creating window...");

            sf::ContextSettings settings;
            settings.depthBits = 24;
            settings.stencilBits = 8;
            settings.antialiasingLevel = 0;
            settings.majorVersion = 2;
            settings.minorVersion = 1;

            sf::RenderWindow renderWindow;
            renderWindow.create(sf::VideoMode(1280, 720), "GUESS Engine - Cube Scene Test", sf::Style::Default, settings);

            // Verify window was created successfully
            if (!renderWindow.isOpen()) {
                Logger::log(Logger::ERROR, "Failed to create render window");
                return;
            }

            Logger::log(Logger::INFO, "Window created successfully");

            // Log actual OpenGL settings
            auto actualSettings = renderWindow.getSettings();
            Logger::log(Logger::INFO, "OpenGL " + std::to_string(actualSettings.majorVersion) + "." + std::to_string(actualSettings.minorVersion));
            Logger::log(Logger::INFO, "Depth bits " + std::to_string(actualSettings.depthBits));

            // Set basic window properties
            // Enable VSync for smooth rendering without flickering
            renderWindow.setVerticalSyncEnabled(true);

            Logger::log(Logger::INFO, "VSync enabled");

            // Set target window
            m_renderManager.SetTargetWindow(&renderWindow);

            Logger::log(Logger::INFO, "Set target window");

            Scene& currentScene = m_sceneManager.getCurrentScene();
            currentScene.getMainCamera().setViewportSize(
                static_cast<float>(renderWindow.getSize().x),
                static_cast<float>(renderWindow.getSize().y)
            );

            // Test if we can actually render - this will tell us if GL context is valid
            Logger::log(Logger::INFO, "Attempting first clear...");

            renderWindow.clear(sf::Color::Black);

            Logger::log(Logger::INFO, "Clear successful, attempting display...");

            renderWindow.display();

            Logger::log(Logger::INFO, "Initial render successful");

            m_isRunning = true;

            while (m_isRunning && renderWindow.isOpen()) {
                sf::Event event;
                while (renderWindow.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        renderWindow.close();
                        stop();
                    }
                    else if (event.type == sf::Event::Resized) {
                        currentScene.getMainCamera().setViewportSize(
                            static_cast<float>(event.size.width),
                            static_cast<float>(event.size.height)
                        );
                    }
                }

                // Update timing
                m_timingSystem.TimingUpdate();
                m_deltaTime = m_timingSystem.DeltaTime();

                // Accumulate time for fixed updates
                m_accumulatedTime += m_deltaTime;

                // Handle fixed time step updates
                while (m_accumulatedTime >= FIXED_TIME_STEP) {
                    fixedUpdate(FIXED_TIME_STEP);
                    m_accumulatedTime -= FIXED_TIME_STEP;
                }

                // Regular update cycle
                update();

                // Late update cycle
                lateUpdate();
            }
        } catch (const std::exception& e) {
            Logger::log(Logger::ERROR, std::string("Exception in game loop: ") + e.what());
        }
    }

    void Engine::stop() {
        m_isRunning = false;
    }

    void Engine::update() {
        m_inputSystem.Update();
        m_sceneManager.getCurrentScene().setInputSystem(&m_inputSystem);
        m_sceneManager.Update();

        Scene& currentScene = m_sceneManager.getCurrentScene();

        // Set the scene's camera to the render manager
        m_renderManager.SetMainCameraPtr(&currentScene.getMainCamera());
        m_renderManager.ClearLights();

        // Process physics components
        for (const auto& [guid, gameObject] : currentScene.GetGameObjects()) {
            // Handle 3D physics
            if (auto* rb3d = gameObject->getComponent<GUESS::core::RigidbodyComponent>()) {
                if (auto* collider = rb3d->getRigidbody()->getCollider()) {
                    collider->setPosition(gameObject->getTransform().getPosition());
                    collider->setRotation(gameObject->getTransform().getRotation().toEuler().y);

                    // Debug pass: submit collider wireframe in world space.
                    m_renderManager.SubmitColliderWireframe(
                        collider->getCenter(),
                        collider->getDimensions(),
                        collider->getScale()
                    );
                }
            }

            // Submit collider debug from component directly as a fallback path.
            if (auto* boxColComp = gameObject->getComponent<GUESS::core::BoxColliderComponent>()) {
                if (boxColComp->collider) {
                    m_renderManager.SubmitColliderWireframe(
                        boxColComp->collider->getCenter(),
                        boxColComp->collider->getDimensions(),
                        boxColComp->collider->getScale()
                    );
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

            if (auto* lightComponent = gameObject->getComponent<LightComponent>()) {
                lightComponent->update();
                if (auto* light = lightComponent->getLight()) {
                    m_renderManager.SubmitLight(*light);
                }
            }

            // Handle rendering
            if (auto* meshRenderer = gameObject->getComponent<MeshRendererComponent>()) {
                if (auto mesh = meshRenderer->getMesh()) {
                    auto rotation = gameObject->getTransform().getRotation();
                    const auto deltaRotation = GUESS::core::math::Quaternion::fromEuler(
                        0.0f,
                        GUESS::core::math::toRadians(45.0f) * m_deltaTime,
                        0.0f
                    );
                    rotation = rotation * deltaRotation;
                    rotation.normalize();
                    gameObject->getTransform().setRotation(rotation);

                    const auto euler = rotation.toEuler();
                    const auto position = gameObject->getTransform().getPosition();
                    const auto scale = gameObject->getTransform().getScale();
                    mesh->setPosition(position);
                    mesh->setRotation(euler);
                    mesh->setScale(scale);

                    auto material = meshRenderer->getMaterial();
                    m_renderManager.SubmitMesh(mesh.get(), material ? material.get() : nullptr);
                }
            }
        }

        m_renderManager.Update();
    }



    void Engine::fixedUpdate(float fixedDeltaTime) {
        // Advance scene fixed time
        m_sceneManager.getCurrentScene().fixedUpdate(fixedDeltaTime);

        // Step physics world
        Scene& currentScene = m_sceneManager.getCurrentScene();
        currentScene.getPhysicsWorld()->step(fixedDeltaTime);

        // Call fixedUpdate on all game objects so components can respond
        for (const auto& [guid, gameObject] : currentScene.GetGameObjects()) {
            gameObject->fixedUpdate();
        }
    }

    void Engine::lateUpdate() {
        // Watch this space
    }
}
