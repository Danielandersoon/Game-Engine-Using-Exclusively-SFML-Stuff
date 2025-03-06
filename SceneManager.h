#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H
#include <string>
#include <memory>
#include "./Manager.h"

namespace GUESS::core {
    class Scene;
    class SceneManager : public Manager {
    private:
        std::vector<std::unique_ptr<Scene>> m_Scenes;
        Scene* m_activeScene;
        unsigned int m_SceneCounter = 0;
        float m_transitionDuration = 1.0f;
        float m_transitionTimer = 0.0f;
        Scene* m_nextScene = nullptr;
        bool m_isTransitioning = false;

    public:
        SceneManager() : m_activeScene(nullptr) {}
        virtual ~SceneManager() override = default;

        virtual bool Initialize() override;
        virtual bool Shutdown() override;
        virtual bool Update() override;
        virtual bool IsInitialized() const override { return m_initialized; }
        virtual bool IsRunning() const override { return m_running; }

        bool CreateNewScene(std::string sceneName);
        bool LoadScene(const std::string& sceneName);
        Scene* GetActiveScene();
        void addScene(Scene&& scene);
        bool DestroyScene(int SceneID);
        bool DestroySceneByName(std::string sceneName);
        unsigned int GetSceneID(Scene* scene_ptr);
        std::vector<int> GetAllSceneIDs();
        unsigned int GetSceneIDByName(std::string sceneName);

        void setTransitionDuration(float duration) { m_transitionDuration = duration; }
        bool transitionToScene(const std::string& sceneName);

        Scene& getCurrentScene();
    };
}
#endif
