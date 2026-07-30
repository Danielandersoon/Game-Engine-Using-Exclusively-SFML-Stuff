#include "./Scene.h"
#include "./Logger.h"

namespace GUESS::core {
    Scene::Scene(std::string sceneName) : m_sceneName(sceneName) {
        m_mainCamera = std::make_unique<GUESS::rendering::Camera>(720.0f, 1280.0f, 1000.0f, 0.1f, 90.0f);
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
            // Initialize physics systems
            m_physicsManager.initializeDefaultMaterials();
            return true;
        }
        return false;
    }

    bool Scene::CloseScene() {
        if (m_active) {
            m_active = false;
            // WARNING: This clears all GameObjects! Scenes lose all objects on transition.
            // If you need to preserve scene state, implement serialization/deserialization
            // or use a different scene management strategy.
            GameObjects.clear();
            return true;
        }
        return false;
    }

    void Scene::AddGameObject(std::unique_ptr<GameObject> gameObject) {
        if (!gameObject) {
            Logger::log(Logger::WARNING, "AddGameObject called with null gameObject");
            return;
        }

        int guid = gameObject->getGUID(); // save before move
        GameObjects.emplace(guid, std::move(gameObject));
        // Assign scene pointer to components so they can access scene APIs safely
        GameObjects[guid]->setOwnerScene(this);
        Logger::log(Logger::INFO, "Object added as " + std::to_string(guid));
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

    GameObject* Scene::FindGameObject(int guid) {
        auto it = GameObjects.find(guid);
        return it != GameObjects.end() ? it->second.get() : nullptr;
    }
}
