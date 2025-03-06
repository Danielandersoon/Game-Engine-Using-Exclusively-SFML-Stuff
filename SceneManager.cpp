#include "./SceneManager.h"
#include "./Scene.h"
#include "./Logger.h"

namespace GUESS::core {
    bool SceneManager::Initialize() {
        if (!m_initialized) {
            m_initialized = true;
            m_running = true;
            m_activeScene = nullptr;
            CreateNewScene("DefaultScene");
            return LoadScene("DefaultScene");
        }
        Logger::log(GUESS::core::Logger::ERROR, "Could not initialize the scene manager");
        return true;
    }

    bool SceneManager::CreateNewScene(std::string sceneName) {
        try {
            m_SceneCounter++;
            m_Scenes.push_back(std::make_unique<Scene>(std::move(sceneName)));
            GUESS::core::Logger::log(GUESS::core::Logger::INFO, "New Scene " + sceneName + " created.");
            return true;
        }
        catch (int e) {
            Logger::log(GUESS::core::Logger::ERROR, "Could not create new scene " + sceneName);
            return false;
        }
    }

    bool SceneManager::LoadScene(const std::string& sceneName) {
        for (auto& scene : m_Scenes) {
            if (scene->GetSceneName() == sceneName) {
                if (m_activeScene) {
                    m_activeScene->CloseScene();
                }
                m_activeScene = scene.get();
                return m_activeScene->LoadScene();
            }
        }
        return false;
    }
    Scene* SceneManager::GetActiveScene() {
        return m_activeScene;
    }

    void SceneManager::addScene(Scene&& scene)
    {
        auto localscene = std::make_unique<Scene>(scene.GetSceneName());
        localscene.get()->setActive(scene.IsActive());
        localscene.get()->setInputSystem(scene.getInputSystem());
        localscene.get()->setMainCamera(std::make_unique<GUESS::rendering::Camera>(scene.getMainCamera()));
        localscene.get()->setPhysicsWorld(std::move(*scene.getPhysicsWorld()));
        localscene.get()->setPhysicsManager(std::move(*scene.getPhysicsManager()));
        m_Scenes.emplace_back(std::move(localscene));

    }

    Scene& SceneManager::getCurrentScene()
    {
        if (m_Scenes.empty()) {
            m_Scenes.emplace_back(std::move(std::make_unique<Scene>("defaultScene")));
        } else if (m_activeScene == nullptr) {
            m_activeScene = m_Scenes[0].get();
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

    bool SceneManager::transitionToScene(const std::string& sceneName) {
        // Find the target scene
        for (auto& scene : m_Scenes) {
            if (scene->GetSceneName() == sceneName) {
                m_nextScene = scene.get();
                m_isTransitioning = true;
                m_transitionTimer = 0.0f;
                return true;
            }
        }
        return false;
    }

    bool SceneManager::Update() {
        if (!m_initialized || !m_running) {
            return false;
        }

        if (m_isTransitioning) {
            m_transitionTimer += 1.0f / 60.0f; // Assuming 60 FPS, use actual delta time in practice
            float t = m_transitionTimer / m_transitionDuration;

            if (t >= 1.0f) {
                // Transition complete
                if (m_activeScene) {
                    m_activeScene->CloseScene();
                }
                m_activeScene = m_nextScene;
                m_activeScene->LoadScene();
                m_isTransitioning = false;
                m_nextScene = nullptr;
            }
        }

        // Update active scene's game objects
        if (m_activeScene) {
            for (auto& gameObject : m_activeScene->GetGameObjects()) {
                if (gameObject.second->isActive()) {
                    gameObject.second->update();
                }
            }
        }

        return true;
    }

}
