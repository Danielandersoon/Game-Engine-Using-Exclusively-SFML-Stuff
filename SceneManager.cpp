#include "SceneManager.h"

namespace GUESS::core {
    bool SceneManager::Initialize() {
        if (!m_initialized) {
            m_initialized = true;
            m_running = true;
            m_activeScene = nullptr;
            return CreateNewScene("DefaultScene");
        }
        return true;
    }

    bool SceneManager::CreateNewScene(std::string sceneName) {
        try {
            m_SceneCounter++;
            m_Scenes.emplace_back(sceneName);
            return true;
        }
        catch (...) {
            return false;
        }
    }

    bool SceneManager::LoadScene(const std::string& sceneName) {
        for (auto& scene : m_Scenes) {
            if (scene.GetSceneName() == sceneName) {
                if (m_activeScene) {
                    m_activeScene->CloseScene();
                }
                m_activeScene = &scene;
                return m_activeScene->LoadScene();
            }
        }
        return false;
    }

    Scene* SceneManager::GetActiveScene() {
        return m_activeScene;
    }

    bool GUESS::core::SceneManager::Shutdown() {
        if (m_initialized) {
            m_Scenes.clear();
            m_activeScene = nullptr;
            m_SceneCounter = 0;
            m_initialized = false;
            m_running = false;
            return true;
        }
        return false;
    }

    bool SceneManager::Update() {
        if (m_initialized && m_running && m_activeScene) {
            // Update active scene's game objects
            for (auto& gameObject : m_activeScene->GetGameObjects()) {
                if (gameObject -> isActive()) {
                    gameObject -> update();
                }
            }
            return true;
        }
        return false;
    }
}
