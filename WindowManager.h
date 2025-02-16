#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H
#include <SFML/Graphics/RenderWindow.hpp>
#include "Manager.h"
namespace GUESS::core {

	class WindowManager : Manager
	{
	private:
		struct WindowData {
			std::unique_ptr<sf::Window> window;
			int id;
		};
		int m_windowCounter = 0;
		std::vector<WindowData> m_WindowDataVec;

	public:
		WindowManager();
		virtual ~WindowManager() override = default;
		virtual bool Initialize() override;
		virtual bool Shutdown() override;
		virtual bool Update() override;
		virtual bool IsInitialized() const override { return m_initialized; }
		virtual bool IsRunning() const override { return m_running; }
		sf::Window* getWindow(int windowID) {
			for (const auto& windowData : m_WindowDataVec) {
				if (windowData.id == windowID) {
					return windowData.window.get();
				}
			}
			return nullptr;
		}

		bool CreateNewWindow(sf::VideoMode vidMode, sf::Vector2i startPosition, std::string windowName);
		bool DestroyWindow(int WindowID);
		unsigned int GetWindowID(sf::Window* window_ptr);
		std::vector<int> GetAllWindowIDs();
	};
}

#endif