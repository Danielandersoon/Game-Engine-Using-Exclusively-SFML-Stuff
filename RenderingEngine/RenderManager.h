#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H
#include <SFML/Graphics.hpp>
#include "../Manager.h"
#include "Camera.h"

namespace GUESS::rendering {
    class RenderManager : public GUESS::core::Manager {
    private:
        std::unique_ptr<Camera> m_mainCamera;
        std::vector<sf::Drawable*> m_renderQueue;
        sf::RenderWindow* m_targetWindow;
        bool m_isVsyncEnabled;

    public:
        RenderManager();
        virtual ~RenderManager() override = default;

        virtual bool Initialize() override;
        virtual bool Shutdown() override;
        virtual bool Update() override;
        virtual bool IsInitialized() const override { return m_initialized; }
        virtual bool IsRunning() const override { return m_running; }

        void SetTargetWindow(sf::RenderWindow* window);
        void SetMainCamera(std::unique_ptr<Camera> camera);
        Camera* GetMainCamera() const { return m_mainCamera.get(); }

        void Submit(sf::Drawable* drawable);
        void ClearQueue();
        void Draw();

        void EnableVsync(bool enable);
        bool IsVsyncEnabled() const { return m_isVsyncEnabled; }
    };
}
#endif
