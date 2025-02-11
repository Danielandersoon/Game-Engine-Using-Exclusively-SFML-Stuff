#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H
#include <SFML/Graphics/RenderWindow.hpp>
#include "Manager.h"
namespace GUESS::core {

	class WindowManager : Manager
	{
	private:
		std::vector<sf::Window> m_renderWindows;
		int m_windowCounter = 0;

	public:
		WindowManager();
		virtual ~WindowManager() override = default;
		virtual bool Initialize() override;
		virtual bool Shutdown() override;
		virtual bool Update() override;
		virtual bool IsInitialized() const override { return m_initialized; }
		virtual bool IsRunning() const override { return m_running; }

		bool CreateNewWindow(sf::VideoMode vidMode, sf::Vector2i startPosition, std::string windowName);
		bool DestroyWindow(int WindowID);
		unsigned int GetWindowID(sf::RenderWindow* renderWindow_ptr);
		std::vector<int> GetAllWindowIDs();
	};
}

#endif