#include "./Scene.h"

#include "./Logger.h"

namespace GUESS::core {
    Scene::Scene(std::string sceneName) : m_sceneName(sceneName) , m_mainCamera(900.0f, 1440.0f, 1000.0f, 0.1f, 60.0f) {
        static unsigned int nextID = 0;
        ID = ++nextID;
        m_active = false;
    }

    bool Scene::LoadScene() {
        if (!m_active) {
            m_active = true;
            // Initialize all GameObjects
            for (auto& [guid, gameObject] : GameObjects) {
                gameObject->update();
            }
            return true;
        }
        return false;
    }

    bool Scene::CloseScene() {
        if (m_active) {
            m_active = false;
            GameObjects.clear();
            return true;
        }
        return false;
    }

    void Scene::AddGameObject(std::unique_ptr<GameObject> gameObject) {
        Logger::log(Logger::INFO, "Created new game object: " + gameObject->getName());
        int guid = gameObject->getGUID();
        GameObjects[guid] = std::move(gameObject);
    }

    void Scene::RemoveGameObject(const std::string& objectName) {
        for (auto it = GameObjects.begin(); it != GameObjects.end(); ++it) {
            if (it->second->getName() == objectName) {
                GameObjects.erase(it);
                Logger::log(Logger::INFO, "Removed game object: " + objectName);
                return;
            }
        }
        Logger::log(Logger::WARNING, "Could not find object to remove: " + objectName);
    }

    
    GameObject* Scene::FindGameObject(const std::string& objectName) {
        auto it = std::find_if(GameObjects.begin(), GameObjects.end(),
            [&objectName](const auto& pair) {
            return pair.second->getName() == objectName;
        });
        return it != GameObjects.end() ? it->second.get() : nullptr;
    }

    GameObject* Scene::FindGameObject(int guid)
    {
        auto it = GameObjects.find(guid);
        return it != GameObjects.end() ? it->second.get() : nullptr;
    }
}
