#include "Scene.h"

namespace GUESS::core {
    Scene::Scene(std::string sceneName) : m_sceneName(sceneName) {
        static unsigned int nextID = 0;
        ID = ++nextID;
        m_active = false;
    }

    bool Scene::LoadScene() {
        if (!m_active) {
            m_active = true;
            // Initialize all GameObjects
            for (auto& gameObject : GameObjects) {
                gameObject -> update();
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
        GameObjects.push_back(std::move(gameObject));
    }

    void Scene::RemoveGameObject(const std::string& objectName) {
        GameObjects.erase(
            std::remove_if(GameObjects.begin(), GameObjects.end(),
                [&objectName](const std::unique_ptr<GameObject>& obj) {
            return obj->getName() == objectName;
        }),
            GameObjects.end());
    }

    GameObject* Scene::FindGameObject(const std::string& objectName) {
        auto it = std::find_if(GameObjects.begin(), GameObjects.end(),
            [&objectName](const std::unique_ptr<GameObject>& obj) {
            return obj->getName() == objectName;
        });
        return it != GameObjects.end() ? it->get() : nullptr;
    }
}
