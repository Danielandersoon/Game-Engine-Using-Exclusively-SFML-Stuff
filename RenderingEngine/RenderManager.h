#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "../Manager.h"
#include "Camera.h"
#include "Material.h"
#include "Mesh.h"

namespace GUESS::rendering {
    class RenderManager : public GUESS::core::Manager {
    private:
        std::unique_ptr<Camera> m_mainCamera;
        std::vector<sf::Drawable*> m_renderQueue;
        sf::RenderWindow* m_targetWindow;
        bool m_isVsyncEnabled;
        std::vector<GUESS::rendering::threed::Mesh*> m_meshRenderQueue;
        std::unordered_map<unsigned int, sf::Shader> m_shaderCache;
        sf::RenderStates m_currentRenderState;

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

        void SubmitMesh(GUESS::rendering::threed::Mesh* mesh);
        void SetShader(const std::string& vertexShader, const std::string& fragmentShader);
        void DrawMesh(const GUESS::rendering::threed::Mesh& mesh, const GUESS::core::math::Matrix4x4& transform);
    
        void setupMeshRendering(const Material& material);
        void renderMesh(const GUESS::rendering::threed::Mesh& mesh, const Material& material);

    };
}
#endif
