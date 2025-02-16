#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H
#include <string>
#include "Manager.h"
#include "Scene.h"

namespace GUESS::core {
    class SceneManager : public Manager {
    private:
        std::vector<Scene> m_Scenes;
        Scene* m_activeScene;
        unsigned int m_SceneCounter = 0;


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
        bool DestroyScene(int SceneID);
        bool DestroySceneByName(std::string sceneName);
        unsigned int GetSceneID(Scene* scene_ptr);
        std::vector<int> GetAllSceneIDs();
        unsigned int GetSceneIDByName(std::string sceneName);

        Scene& getCurrentScene();
    };
}
#endif
