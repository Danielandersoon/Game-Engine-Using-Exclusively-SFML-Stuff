#ifndef ENGINE_H
#define ENGINE_H
#include "InputSystem.h"
#include "TimingSystem.h"
#include "SceneManager.h"
#include "WindowManager.h"

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

        // Fixed timestep settings
        const float FIXED_TIME_STEP = 1.0f / 60.0f;
        float m_accumulatedTime = 0.0f;

    public:
        Engine(int instanceID);
        unsigned int getInstanceID();

        void start();
        void stop();
        void update();
        void fixedUpdate();
        void lateUpdate();

        bool isRunning() const { return m_isRunning; }
    };
}

#endif
