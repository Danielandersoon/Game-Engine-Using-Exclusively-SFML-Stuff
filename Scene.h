#ifndef SCENE_H
#define SCENE_H
#include <string>
#include <algorithm>
#include "GameObject.h"

namespace GUESS::core {
    class Scene {
    public:
        Scene(std::string sceneName);

        unsigned int GetSceneID() const { return ID; }
        std::string GetSceneName() const { return m_sceneName; }
        bool IsActive() const { return m_active; }

        bool LoadScene();
        bool CloseScene();

        void AddGameObject(std::unique_ptr<GameObject> gameObject);
        void RemoveGameObject(const std::string& objectName);
        GameObject* FindGameObject(const std::string& objectName);

        const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const { return GameObjects; }

    private:
        std::vector<std::unique_ptr<GameObject>> GameObjects;
        unsigned int ID;
        std::string m_sceneName;
        bool m_active;
    };
}
#endif
