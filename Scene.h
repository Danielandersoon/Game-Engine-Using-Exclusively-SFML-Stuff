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
        std::unique_ptr<GUESS::rendering::Camera> m_mainCamera;
        GUESS::core::InputSystem* m_inputSystem;
        float m_deltaTime;
        float m_fixedDeltaTime;

        // Pysics systems
        GUESS::physics::PhysicsWorld m_physicsWorld;
        GUESS::physics::PhysicsManager m_physicsManager;

    public:
        Scene(std::string sceneName);
        Scene& operator=(Scene&& other) noexcept {
            if (this != &other) {
                m_mainCamera = std::move(other.m_mainCamera);
                GameObjects = std::move(other.GameObjects);
                m_sceneName = std::move(other.m_sceneName);
                m_active = other.m_active;
                ID = other.ID;
            }
            return *this;
        }

        Scene(Scene&& other) noexcept
            : m_mainCamera(std::move(other.m_mainCamera))
            , GameObjects(std::move(other.GameObjects))
            , m_sceneName(std::move(other.m_sceneName))
            , m_active(other.m_active)
            , ID(other.ID) {}

        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;

        unsigned int GetSceneID() const { return ID; }
        std::string GetSceneName() const { return m_sceneName; }
        InputSystem* getInputSystem() const { return m_inputSystem; }
        GUESS::physics::PhysicsManager* getPhysicsManager() { return &m_physicsManager; }
        GUESS::physics::PhysicsWorld* getPhysicsWorld() { return &m_physicsWorld; }
        const std::unordered_map<int, std::unique_ptr<GameObject>>& GetGameObjects() const { return GameObjects; }
        GUESS::rendering::Camera& getMainCamera() { 
            if (!m_mainCamera) {
                throw std::runtime_error("Scene main camera is null");
            }
            return *m_mainCamera; 
        }

        bool IsActive() const { return m_active; }

        bool LoadScene();
        bool CloseScene();

        void AddGameObject(std::unique_ptr<GameObject> gameObject);
        void RemoveGameObject(const std::string& objectName);
        GameObject* FindGameObject(const std::string& objectName);
        GameObject* FindGameObject(int guid);

        void setInputSystem(InputSystem* input) { m_inputSystem = input; }
        void setSceneName(const std::string& name) { m_sceneName = name; }
        void setActive(bool active) { m_active = active; }
        void setPhysicsWorld(GUESS::physics::PhysicsWorld&& world) { m_physicsWorld = std::move(world); }
        void setPhysicsManager(GUESS::physics::PhysicsManager&& manager) { m_physicsManager = std::move(manager); }
        void setMainCamera(std::unique_ptr<GUESS::rendering::Camera> camera) { m_mainCamera = std::move(camera); }

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
