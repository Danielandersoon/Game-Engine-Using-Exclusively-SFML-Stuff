#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "../Manager.h"
#include "./Camera.h"
#include "./Material.h"
#include "./Mesh.h"
#include "./LightManager.h"

namespace GUESS::rendering {
    class RenderManager : public GUESS::core::Manager {
    private:
        struct MeshRenderItem {
            GUESS::rendering::threed::Mesh* mesh = nullptr;
            GUESS::rendering::Material* material = nullptr;
        };

        struct ColliderWireframeItem {
            GUESS::core::math::Vector3f center;
            GUESS::core::math::Vector3f dimensions;
            GUESS::core::math::Vector3f scale;
            GUESS::core::math::Quaternion orientation;
        };

        std::unique_ptr<Camera> m_mainCamera;
        Camera* m_cameraPtr = nullptr;
        std::vector<sf::Drawable*> m_renderQueue;
        sf::RenderWindow* m_targetWindow;
        bool m_isVsyncEnabled;
        std::vector<MeshRenderItem> m_meshRenderQueue;
        std::vector<ColliderWireframeItem> m_colliderWireframeQueue;
        std::unordered_map<unsigned int, sf::Shader> m_shaderCache;
        sf::RenderStates m_currentRenderState;
        GUESS::rendering::Shader m_defaultMeshShader;
        GUESS::rendering::threed::LightManager m_lightManager;

        void drawColliderWireframes();

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
        void SetMainCameraPtr(Camera* camera) {
            if (camera) {
                m_cameraPtr = camera;
            }
        }
        Camera* GetMainCamera() const { return m_cameraPtr ? m_cameraPtr : m_mainCamera.get(); }

        void Submit(sf::Drawable* drawable);
        void ClearQueue();
        void Draw();

        void EnableVsync(bool enable);
        bool IsVsyncEnabled() const { return m_isVsyncEnabled; }

        void SubmitMesh(GUESS::rendering::threed::Mesh* mesh, GUESS::rendering::Material* material = nullptr);
        void SubmitColliderWireframe(const GUESS::core::math::Vector3f& center,
                         const GUESS::core::math::Vector3f& dimensions,
                         const GUESS::core::math::Vector3f& scale,
                         const GUESS::core::math::Quaternion& orientation);
        void SubmitLight(const GUESS::rendering::threed::Light& light) { m_lightManager.addLight(light); }
        void ClearLights() { m_lightManager.clearLights(); }
        void SetShader(const std::string& vertexShader, const std::string& fragmentShader);
        void DrawMesh(const GUESS::rendering::threed::Mesh& mesh, const GUESS::core::math::Matrix4x4& transform, GUESS::rendering::Material* material);

        void setupMeshRendering(const Material& material);
        void renderMesh(const GUESS::rendering::threed::Mesh& mesh, const Material& material);
    };
}
#endif
