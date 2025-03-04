#include "./WindowManager.h"

#include "./Logger.h"

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
            GUESS::core::Logger::log(GUESS::core::Logger::INFO, "windowManager Initialized sucessfuly");
            return CreateNewWindow(sf::VideoMode(640, 480, 32), sf::Vector2i(0, 0), "initWin");
        }
        Logger::log(GUESS::core::Logger::ERROR, "Could not initialize the window manager");
        return true;
    }

    bool WindowManager::Shutdown()
    {
        if (m_initialized) {
            cleanupAllWindows();
            m_windowCounter = 0;
            m_initialized = false;
            m_running = false;
            GUESS::core::Logger::log(GUESS::core::Logger::INFO, "WindowManager shutdown successfully");
        }
        return true;
    }

    bool WindowManager::DestroyWindow(int WindowID)
    {
        try {
            auto it = std::find_if(m_WindowDataVec.begin(), m_WindowDataVec.end(),
                [WindowID](const WindowData& data) { return data.id == WindowID; });

            if (it != m_WindowDataVec.end()) {
                it->cleanup();
                m_WindowDataVec.erase(it);
                GUESS::core::Logger::log(GUESS::core::Logger::INFO,
                    "Window ID: " + std::to_string(WindowID) + " destroyed");
                return true;
            }
            return false;
        }
        catch (const std::exception& e) {
            Logger::log(GUESS::core::Logger::ERROR,
                "Could not Destroy window ID: " + std::to_string(WindowID) +
                " Error: " + std::string(e.what()));
            return false;
        }
    }


    bool WindowManager::CreateNewWindow(sf::VideoMode vidMode, sf::Vector2i startPosition, std::string windowName) {
        try {
            m_windowCounter++;
            WindowData winDat;
            winDat.id = m_windowCounter;
            winDat.window = std::make_unique<sf::Window>(vidMode, windowName);
            winDat.window->setPosition(startPosition);
            m_WindowDataVec.push_back(std::move(winDat));
            GUESS::core::Logger::log(GUESS::core::Logger::INFO, "New Window " + windowName + " created");
            return true;
        }
        catch (const std::exception& e) {
            Logger::log(GUESS::core::Logger::ERROR, "Window creation failed: " + std::string(e.what()));
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