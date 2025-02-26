#ifndef ENGINE_H
#define ENGINE_H

// Core system headers
#include "GameObject.h"
#include "Scene.h"
#include "Logger.h"
#include "EventSystem.h"
#include "InputSystem.h"
#include "TimingSystem.h"
#include "SceneManager.h"
#include "WindowManager.h"
#include "ResourceManager.h"

// Rendering system headers
#include "./RenderingEngine/RenderManager.h"
#include "./RenderingEngine/RenderingPipeline.h"

// Physics system headers
#include "./PhysicsEngine/PhysicsWorld.h"
#include "./PhysicsEngine/PhysicsManager.h"
#include "./PhysicsEngine/Rigidbody2D.h"


namespace GUESS::core {

    class Engine {
    private:
        unsigned int m_instanceID;
        bool m_isRunning = false;

        // Core systems
        InputSystem m_inputSystem;
        TimingSystem m_timingSystem;
        SceneManager m_sceneManager;
        WindowManager m_windowManager;
        EventSystem m_eventSystem;
        static Logger m_logger;

        // Rendering systems
        GUESS::rendering::RenderManager m_renderManager;
        GUESS::rendering::RenderingPipeline m_renderingPipeline;

        // Pysics systems
        GUESS::physics::PhysicsWorld m_physicsWorld;
        GUESS::physics::PhysicsManager m_physicsManager;

        // Fixed timestep settings
        const float FIXED_TIME_STEP = 1.0f / 60.0f;
        float m_accumulatedTime = 0.0f;

    public:
        Engine(int instanceID);
        Engine(Engine&&) = default;                    
        Engine& operator=(Engine&&) = default;         
        Engine(const Engine&) = delete;                
        Engine& operator=(const Engine&) = delete;     
        unsigned int getInstanceID();

        void start();
        void stop();
        void update();
        void fixedUpdate();
        void lateUpdate();

        bool isRunning() const { return m_isRunning; }

        SceneManager* getSceneManager() { return &m_sceneManager; };
    };
}

#endif
