#ifndef SCENE_H
#define SCENE_H
#include <string>
#include <algorithm>
#include "./RenderingEngine/Camera.h"
#include "GameObject.h"

namespace GUESS::core {
    class Scene {

    private:
        std::vector<std::unique_ptr<GameObject>> GameObjects;
        unsigned int ID;
        std::string m_sceneName;
        bool m_active;
        GUESS::rendering::Camera m_mainCamera;
    public:
        Scene(std::string sceneName);

        unsigned int GetSceneID() const { return ID; }
        std::string GetSceneName() const { return m_sceneName; }
        bool IsActive() const { return m_active; }
        GUESS::rendering::Camera& getMainCamera() const { return m_mainCamera; }

        bool LoadScene();
        bool CloseScene();

        void AddGameObject(std::unique_ptr<GameObject> gameObject);
        void RemoveGameObject(const std::string& objectName);
        GameObject* FindGameObject(const std::string& objectName);

        const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const { return GameObjects; }

    };
}
#endif
