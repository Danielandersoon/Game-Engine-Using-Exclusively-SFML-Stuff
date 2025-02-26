#ifndef SCENE_H
#define SCENE_H
#include <string>
#include <algorithm>
#include "./InputSystem.h"
#include "./RenderingEngine/Camera.h"
#include "GameObject.h"

namespace GUESS::core {
    class Scene {

    protected:
        std::vector<std::unique_ptr<GameObject>> GameObjects;
        unsigned int ID;
        std::string m_sceneName;
        bool m_active;
        GUESS::rendering::Camera m_mainCamera;
        GUESS::core::InputSystem* m_inputSystem;

    public:
        Scene(std::string sceneName);
        Scene(Scene& scene) = default;
        Scene(const Scene& other) = delete; 
        Scene& operator=(const Scene& other) = delete;
        Scene(Scene&& other) noexcept = default; 
        Scene& operator=(Scene&& other) noexcept = default;

        unsigned int GetSceneID() const { return ID; }
        std::string GetSceneName() const { return m_sceneName; }
        bool IsActive() const { return m_active; }
        GUESS::rendering::Camera& getMainCamera() { return m_mainCamera; }

        bool LoadScene();
        bool CloseScene();

        void AddGameObject(std::unique_ptr<GameObject> gameObject);
        void RemoveGameObject(const std::string& objectName);
        GameObject* FindGameObject(const std::string& objectName);

        const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const { return GameObjects; }
        void setInputSystem(InputSystem* input) { m_inputSystem = input; }

        void update() {};

    };
}
#endif
