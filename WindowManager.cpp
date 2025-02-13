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
            m_WindowDataVec.clear();
            m_windowCounter = 0;
            m_initialized = false;
            m_running = false;
        }
        return true;;
    }

    bool WindowManager::CreateNewWindow(sf::VideoMode vidMode, sf::Vector2i startPosition, std::string windowName) {
        try {
            m_windowCounter++;
            WindowData winDat;
            winDat.id = m_windowCounter;
            winDat.window = std::make_unique<sf::Window>(vidMode, windowName);
            winDat.window->setPosition(startPosition);
            m_WindowDataVec.push_back(std::move(winDat));
            return true;
        }
        catch (int e) {
            return false;
        }
    }

    bool WindowManager::Update()
    {
        if (m_initialized && m_running) {
            for (auto& instance : m_WindowDataVec) {
            }
            return true;
        }
        return false;
    }

    bool WindowManager::DestroyWindow(int WindowID)
    {
        try {
            for (int x = 0; x < m_WindowDataVec.size(); x++)
            {
                if (m_WindowDataVec[x].id == WindowID)
                {
                    m_WindowDataVec.erase(m_WindowDataVec.begin() + x);
                    return true;
                }
            }
        }
        catch (int e) {
            return false;
        }
    }

    unsigned int WindowManager::GetWindowID(sf::Window* window_ptr)
    {
        for (const auto& windowData : m_WindowDataVec) {
            if (windowData.window.get() == window_ptr) {
                return windowData.id;
            }
        }
        return 0;
    }

    std::vector<int> WindowManager::GetAllWindowIDs()
    {
        std::vector<int> IdVec;
        for (int x = 0; x < m_WindowDataVec.size(); x++)
        {
            IdVec.push_back(m_WindowDataVec[x].id);
        }
        return IdVec;
    }
}