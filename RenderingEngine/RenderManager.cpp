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

    void RenderManager::Draw() {
        if (!m_targetWindow || !m_mainCamera) return;

        m_targetWindow->clear();
        m_targetWindow->setView(m_mainCamera->getView());

        for (auto* drawable : m_renderQueue) {
            m_targetWindow->draw(*drawable);
        }

        m_targetWindow->display();
        ClearQueue();
    }

    void RenderManager::EnableVsync(bool enable) {
        m_isVsyncEnabled = enable;
        if (m_targetWindow) {
            m_targetWindow->setVerticalSyncEnabled(enable);
        }
    }
}