#include "SceneManager.h"
namespace GUESS::core {

    SceneManager::SceneManager()
    {
        Initialize();
    }

    bool SceneManager::Initialize()
    {
        if (!m_initialized) {
            m_initialized = true;
            m_running = true;
            return CreateNewScene("Scene1");
            return true;
        }
    }

    bool SceneManager::Shutdown()
    {
        if (m_initialized) {
            std::vector<int> instances = GetAllWindowIDs();
            for (int id : instances) {
                DestroyScene(id);
            }
            m_Scenes.clear();
            m_SceneCounter = 0;
            m_initialized = false;
            m_running = false;
        }
        return true;
    }

    bool SceneManager::Update()
    {
        if (m_initialized && m_running) {
            for (auto& instance : m_Scenes) {
            }
            return true;
        }
        return false;
    }

    bool SceneManager::CreateNewScene(std::string sceneName)
    {
        try {
            m_SceneCounter++;
            m_Scenes.push_back(Scene(sceneName));
            return true;
        }
        catch (int e) {
            return false;
        }
    }

    bool SceneManager::DestroyScene(int SceneID)
    {
        try {
            for (int x = 0; x < m_Scenes.size(); x++)
            {
                if (m_Scenes[x].GetSceneID() == SceneID)
                {
                    m_Scenes.erase(m_Scenes.begin() + x);
                    return true;
                }
            }
        }
        catch (int e) {
            return false;
        }
    }

    bool SceneManager::DestroySceneByName(std::string sceneName)
    {
        try {
            for (int x = 0; x < m_Scenes.size(); x++)
            {
                if (m_Scenes[x].GetSceneName() == sceneName)
                {
                    m_Scenes.erase(m_Scenes.begin() + x);
                    return true;
                }
            }
        }
        catch (int e) {
            return false;
        }
    }

    unsigned int SceneManager::GetSceneID(Scene* scene_ptr)
    {
        return scene_ptr->GetSceneID();
    }

    std::vector<int> SceneManager::GetAllWindowIDs()
    {
        std::vector<int> IdVec;
        for (int x = 0; x < m_Scenes.size(); x++)
        {
            IdVec.push_back(m_Scenes[x].GetSceneID());
        }
        return IdVec;
    }

    unsigned int SceneManager::GetSceneIDByName(std::string sceneName)
    {
        try {
            for (int x = 0; x < m_Scenes.size(); x++)
            {
                if (m_Scenes[x].GetSceneName() == sceneName)
                {
                    return m_Scenes[x].GetSceneID();
                }
            }
        }
        catch (int e) {
            return false;
        }
    }
}