#include "Engine.h"

namespace GUESS::core {

    Engine::Engine(int instanceID) : m_instanceID(instanceID) {
        // Initialize managers
        m_sceneManager.Initialize();
        m_windowManager.Initialize();
    }

    unsigned int Engine::getInstanceID() {
        return m_instanceID;
    }

    void Engine::start() {
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
        // Update order: Input -> Scene -> Window
        m_inputSystem.Update();
        m_sceneManager.Update();
        m_windowManager.Update();
    }

    void Engine::fixedUpdate() {
        // Watch this space
    }

    void Engine::lateUpdate() {
        // Watch this space
    }
}
