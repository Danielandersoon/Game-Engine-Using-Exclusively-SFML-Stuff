#ifndef SCENE_H
#define SCENE_H
#include <string>
#include <algorithm>
#include "./InputSystem.h"
#include "./RenderingEngine/Camera.h"
#include "./GameObject.h"
#include "./PhysicsEngine/PhysicsWorld.h"
#include "./PhysicsEngine/PhysicsManager.h"

namespace GUESS::core {
    class Scene {

    protected:
        std::unordered_map<int, std::unique_ptr<GameObject>> GameObjects;
        unsigned int ID;
        std::string m_sceneName;
        bool m_active;
        GUESS::rendering::Camera m_mainCamera;
        GUESS::core::InputSystem* m_inputSystem;
        float m_deltaTime;
        float m_fixedDeltaTime;

        // Pysics systems
        GUESS::physics::PhysicsWorld m_physicsWorld;
        GUESS::physics::PhysicsManager m_physicsManager;

    public:
        Scene(std::string sceneName);
        Scene(Scene& scene) = default;
        Scene(const Scene& other) = delete; 
        Scene& operator=(const Scene& other) = delete;
        Scene(Scene&& other) noexcept = default; 
        Scene& operator=(Scene&& other) noexcept = default;

        unsigned int GetSceneID() const { return ID; }
        std::string GetSceneName() const { return m_sceneName; }

        GUESS::physics::PhysicsWorld* getPhysicsWorld() { return &m_physicsWorld; }

        bool IsActive() const { return m_active; }
        GUESS::rendering::Camera& getMainCamera() { return m_mainCamera; }

        bool LoadScene();
        bool CloseScene();

        void AddGameObject(std::unique_ptr<GameObject> gameObject);
        void RemoveGameObject(const std::string& objectName);
        GameObject* FindGameObject(const std::string& objectName);
        GameObject* FindGameObject(int guid);

        const std::unordered_map<int, std::unique_ptr<GameObject>>& GetGameObjects() const { return GameObjects; };
        void setInputSystem(InputSystem* input) { m_inputSystem = input; }


        //Time methods
        float DeltaTime() { return m_deltaTime; };
        float FixedDeltaTime() { return m_fixedDeltaTime; };

        // Update methods
        void update(float deltaTime) { m_deltaTime = deltaTime; };
        void fixedUpdate(float fixedDeltatime) { m_fixedDeltaTime = fixedDeltatime; };
        void lateUpdate() {};


    };
}
#endif
