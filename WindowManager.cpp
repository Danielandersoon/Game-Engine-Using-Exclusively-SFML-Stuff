#include "WindowManager.h"

namespace GUESS::core {

    WindowManager::WindowManager()
    {
        Initialize();
    }

    bool WindowManager::Initialize()
    {
        if (!m_initialized) {
            m_initialized = true;
            m_running = true;
            return CreateNewWindow(sf::VideoMode(480, 640, 32), sf::Vector2i(0, 0), "initWin");
        }
        return true;
    }

    bool WindowManager::Shutdown()
    {
        if (m_initialized) {
            std::vector<int> instances = GetAllWindowIDs();
            for (int id : instances) {
                DestroyWindow(id);
            }
            m_renderWindows.clear();
            m_windowCounter = 0;
            m_initialized = false;
            m_running = false;
        }
        return true;;
    }

    bool WindowManager::CreateNewWindow(sf::VideoMode vidMode, sf::Vector2i startPosition, std::string windowName)
    {
        try {
            m_windowCounter++;
            sf::Window newWindow(vidMode, windowName);
            newWindow.setPosition(startPosition);
            newWindow.setID(m_windowCounter);
            return true;
        }
        catch (int e) {
            return false;
        }
    }

    bool WindowManager::Update()
    {
        if (m_initialized && m_running) {
            for (auto& instance : m_renderWindows) {
            }
            return true;
        }
        return false;
    }

    bool WindowManager::DestroyWindow(int WindowID)
    {
        try {
            for (int x = 0; x < m_renderWindows.size(); x++)
            {
                if (m_renderWindows[x].GetID() == WindowID)
                {
                    m_renderWindows.erase(m_renderWindows.begin() + x);
                    return true;
                }
            }
        }
        catch (int e) {
            return false;
        }
    }

    unsigned int WindowManager::GetWindowID(sf::RenderWindow* renderWindow_ptr)
    {
        return renderWindow_ptr->GetID();
    }

    std::vector<int> WindowManager::GetAllWindowIDs()
    {
        std::vector<int> IdVec;
        for (int x = 0; x < m_renderWindows.size(); x++)
        {
            IdVec.push_back(m_renderWindows[x].GetID());
        }
        return IdVec;
    }
}