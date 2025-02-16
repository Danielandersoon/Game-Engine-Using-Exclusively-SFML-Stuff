#include "SceneManager.h"
#include "Logger.h"

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
            GUESS::core::Logger::log(GUESS::core::Logger::INFO, "New Scene " + sceneName + " created.");
            return true;
        }
        catch (int e) {
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
                GUESS::core::Logger::log(GUESS::core::Logger::INFO, "Scene " + sceneName + " loaded.");
                return m_activeScene->LoadScene();
            }
        }
        return false;
    }

    Scene* SceneManager::GetActiveScene() {
        return m_activeScene;
    }

    Scene& SceneManager::getCurrentScene()
    {
        if (m_Scenes.empty()) {
            m_Scenes.emplace_back(Scene("defaultScene"));
        } else if (m_activeScene == nullptr) {
            m_activeScene = &m_Scenes[0];
            m_activeScene->LoadScene();
        }
        
    return *m_activeScene;
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
