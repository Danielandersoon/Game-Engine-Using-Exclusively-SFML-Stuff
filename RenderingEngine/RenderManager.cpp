#include "./RenderManager.h"
#include <SFML/OpenGL.hpp>
#include "../Logger.h"
#include <cmath>

namespace GUESS::rendering {
    RenderManager::RenderManager() : m_targetWindow(nullptr), m_isVsyncEnabled(true) {
        Initialize();
    }

    bool RenderManager::Initialize() {
        if (!m_initialized) {
            m_initialized = true;
            m_running = true;
            m_mainCamera = std::make_unique<Camera>(800.0f, 600.0f);

            bool shaderLoaded = m_defaultMeshShader.loadFromFile("shaders/default_mesh.vert", "shaders/default_mesh.frag");
            if (!shaderLoaded) {
                static const char* prefixes[] = { "./", "../", "../../", "../../../" };
                for (const char* prefix : prefixes) {
                    const std::string vert = std::string(prefix) + "shaders/default_mesh.vert";
                    const std::string frag = std::string(prefix) + "shaders/default_mesh.frag";
                    if (m_defaultMeshShader.loadFromFile(vert, frag)) {
                        shaderLoaded = true;
                        break;
                    }
                }
            }

            if (!shaderLoaded) {
                GUESS::core::Logger::log(GUESS::core::Logger::WARNING, "Default mesh shader failed to load.");
            }

            return true;
        }
        return true;
    }

    bool RenderManager::Shutdown() {
        if (m_initialized) {
            ClearQueue();
            m_mainCamera.reset();
            m_targetWindow = nullptr;
            m_initialized = false;
            m_running = false;
            return true;
        }
        return false;
    }

    bool RenderManager::Update() {
        if (m_initialized && m_running && m_targetWindow && m_targetWindow->isOpen()) {
            Draw();
            return true;
        }
        return false;
    }

    void RenderManager::SetTargetWindow(sf::RenderWindow* window) {
        m_targetWindow = window;
    }

    void RenderManager::SetMainCamera(std::unique_ptr<Camera> camera) {
        m_mainCamera = std::move(camera);
    }

    void RenderManager::Submit(sf::Drawable* drawable) {
        if (drawable) {
            m_renderQueue.push_back(drawable);
        }
    }

    void RenderManager::ClearQueue() {
        m_renderQueue.clear();
    }

    void RenderManager::EnableVsync(bool enable) {
        m_isVsyncEnabled = enable;
        if (m_targetWindow) {
            m_targetWindow->setVerticalSyncEnabled(enable);
        }
    }

    void RenderManager::Draw() {
        Camera* activeCamera = m_cameraPtr ? m_cameraPtr : m_mainCamera.get();

        if (!m_targetWindow) return;
        if (!m_targetWindow->isOpen()) return;
        if (!activeCamera) return;

        m_targetWindow->setActive(true);

        const sf::Vector2u windowSize = m_targetWindow->getSize();
        const float aspectRatio = (windowSize.y != 0)
            ? static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y)
            : 1.0f;

        glViewport(0, 0, static_cast<GLsizei>(windowSize.x), static_cast<GLsizei>(windowSize.y));
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LEQUAL);

        glClearColor(30.0f / 255.0f, 30.0f / 255.0f, 40.0f / 255.0f, 1.0f);
        glClearDepth(1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        const float nearPlane = 0.1f;
        const float farPlane = 2000.0f;
        const float fovRadians = GUESS::core::math::toRadians(activeCamera->getFov());
        const float top = nearPlane * std::tan(fovRadians * 0.5f);
        const float right = top * aspectRatio;
        glFrustum(-right, right, -top, top, nearPlane, farPlane);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        const auto& camPos = activeCamera->getPosition();
        glTranslatef(-camPos.x, -camPos.y, -camPos.z);

        m_lightManager.cullLights(camPos);

        for (const auto& item : m_meshRenderQueue) {
            if (item.mesh) {
                DrawMesh(*item.mesh, item.mesh->getTransform(), item.material);
            }
        }

        drawColliderWireframes();

        sf::Shader::bind(nullptr);
        sf::Texture::bind(nullptr);

        m_targetWindow->pushGLStates();
        for (auto* drawable : m_renderQueue) {
            m_targetWindow->draw(*drawable);
        }
        m_targetWindow->popGLStates();

        m_targetWindow->display();
        ClearQueue();
        m_meshRenderQueue.clear();
        m_colliderWireframeQueue.clear();
    }

    void RenderManager::DrawMesh(const GUESS::rendering::threed::Mesh& mesh, const GUESS::core::math::Matrix4x4& transform, GUESS::rendering::Material* material) {
        const auto& meshVertices = mesh.getVertices();
        const auto& indices = mesh.getIndices();

        if (meshVertices.empty() || indices.empty()) {
            return;
        }

        sf::Texture* texture = nullptr;
        if (material) {
            texture = material->getMainTexture();
        }

        GUESS::rendering::Shader* activeShader = nullptr;
        if (material && material->getShader().isValid()) {
            material->bind();
            activeShader = &material->getShader();
        }
        else if (m_defaultMeshShader.isValid()) {
            activeShader = &m_defaultMeshShader;
        }

        if (activeShader) {
            activeShader->setUniform("u_hasTexture", texture ? 1 : 0);
            //activeShader->setUniform("hasTexture", texture ? 1 : 0);
            if (texture) {
                activeShader->setUniform("u_mainTexture", *texture);
                //activeShader->setUniform("mainTexture", *texture);
            }
            m_lightManager.updateLightUniforms(*activeShader);
            activeShader->bind();

            // Debugging: compute lighting for first vertex and log values to help
            // identify why dynamic lighting isn't producing visible results.
            const auto& visibleLights = m_lightManager.getVisibleLights();
            if (!visibleLights.empty() && !meshVertices.empty() && !indices.empty()) {
                const auto& firstVertex = meshVertices[indices[0]];
                auto worldPos0 = transform * firstVertex.position;
                auto worldNormal0 = (transform * firstVertex.normal).normalized();

                for (size_t li = 0; li < visibleLights.size(); ++li) {
                    const auto& L = visibleLights[li];
                    auto Lpos = L.getPosition();
                    auto vec = Lpos - worldPos0;
                    float dist = vec.magnitude();
                    auto Ln = vec.normalized();
                    float ndotl = std::max(0.0f, worldNormal0.dot(Ln));
                    float attenuation = 1.0f / (1.0f + 0.02f * dist * dist);
                    float contrib = ndotl * attenuation * L.getIntensity();
                }
            }
        }

        sf::Texture::bind(texture);

        glBegin(GL_TRIANGLES);
        for (size_t i = 0; i + 2 < indices.size(); i += 3) {
            for (int j = 0; j < 3; ++j) {
                const auto& vertex = meshVertices[indices[i + j]];
                const GUESS::core::math::Vector3f worldPos = transform * vertex.position;

                // Transform normal by model matrix without translation (use 3x3 upper-left)
                const auto& M = transform;
                GUESS::core::math::Vector3f worldNormal(
                    M.m[0][0] * vertex.normal.x + M.m[0][1] * vertex.normal.y + M.m[0][2] * vertex.normal.z,
                    M.m[1][0] * vertex.normal.x + M.m[1][1] * vertex.normal.y + M.m[1][2] * vertex.normal.z,
                    M.m[2][0] * vertex.normal.x + M.m[2][1] * vertex.normal.y + M.m[2][2] * vertex.normal.z
                );
                worldNormal = worldNormal.normalized();

                glColor4ub(vertex.color.r, vertex.color.g, vertex.color.b, vertex.color.a);
                glNormal3f(worldNormal.x, worldNormal.y, worldNormal.z);
                glTexCoord2f(vertex.texCoords.x, vertex.texCoords.y);
                glVertex3f(worldPos.x, worldPos.y, worldPos.z);
            }
        }
        glEnd();

        sf::Shader::bind(nullptr);
        sf::Texture::bind(nullptr);
    }

    void RenderManager::SubmitMesh(GUESS::rendering::threed::Mesh* mesh, GUESS::rendering::Material* material) {
        if (mesh) {
            m_meshRenderQueue.push_back({ mesh, material });
        }
    }

    void RenderManager::SubmitColliderWireframe(const GUESS::core::math::Vector3f& center,
                                                const GUESS::core::math::Vector3f& dimensions,
                                                const GUESS::core::math::Vector3f& scale,
                                                const GUESS::core::math::Quaternion& orientation) {
        m_colliderWireframeQueue.push_back({ center, dimensions, scale, orientation });
    }

    void RenderManager::drawColliderWireframes() {
        if (m_colliderWireframeQueue.empty()) {
            return;
        }

        sf::Shader::bind(nullptr);
        sf::Texture::bind(nullptr);

        glDisable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glLineWidth(2.0f);
        glColor3f(0.1f, 1.0f, 0.1f);

        for (const auto& item : m_colliderWireframeQueue) {
            const auto worldDim = item.dimensions * item.scale;
            const float hx = worldDim.x * 0.5f;
            const float hy = worldDim.y * 0.5f;
            const float hz = worldDim.z * 0.5f;

            // Create local-space corners
            GUESS::core::math::Vector3f corners[8] = {
                GUESS::core::math::Vector3f(-hx, -hy, -hz),  // v0
                GUESS::core::math::Vector3f( hx, -hy, -hz),  // v1
                GUESS::core::math::Vector3f( hx,  hy, -hz),  // v2
                GUESS::core::math::Vector3f(-hx,  hy, -hz),  // v3
                GUESS::core::math::Vector3f(-hx, -hy,  hz),  // v4
                GUESS::core::math::Vector3f( hx, -hy,  hz),  // v5
                GUESS::core::math::Vector3f( hx,  hy,  hz),  // v6
                GUESS::core::math::Vector3f(-hx,  hy,  hz)   // v7
            };

            // Rotate and translate to world space
            GUESS::core::math::Vector3f v[8];
            for (int i = 0; i < 8; i++) {
                v[i] = item.orientation * corners[i] + item.center;
            }

            glBegin(GL_LINES);
            // Bottom
            glVertex3f(v[0].x, v[0].y, v[0].z); glVertex3f(v[1].x, v[1].y, v[1].z);
            glVertex3f(v[1].x, v[1].y, v[1].z); glVertex3f(v[2].x, v[2].y, v[2].z);
            glVertex3f(v[2].x, v[2].y, v[2].z); glVertex3f(v[3].x, v[3].y, v[3].z);
            glVertex3f(v[3].x, v[3].y, v[3].z); glVertex3f(v[0].x, v[0].y, v[0].z);

            // Top
            glVertex3f(v[4].x, v[4].y, v[4].z); glVertex3f(v[5].x, v[5].y, v[5].z);
            glVertex3f(v[5].x, v[5].y, v[5].z); glVertex3f(v[6].x, v[6].y, v[6].z);
            glVertex3f(v[6].x, v[6].y, v[6].z); glVertex3f(v[7].x, v[7].y, v[7].z);
            glVertex3f(v[7].x, v[7].y, v[7].z); glVertex3f(v[4].x, v[4].y, v[4].z);

            // Verticals
            glVertex3f(v[0].x, v[0].y, v[0].z); glVertex3f(v[4].x, v[4].y, v[4].z);
            glVertex3f(v[1].x, v[1].y, v[1].z); glVertex3f(v[5].x, v[5].y, v[5].z);
            glVertex3f(v[2].x, v[2].y, v[2].z); glVertex3f(v[6].x, v[6].y, v[6].z);
            glVertex3f(v[3].x, v[3].y, v[3].z); glVertex3f(v[7].x, v[7].y, v[7].z);
            glEnd();
        }

        glLineWidth(1.0f);
        glDepthMask(GL_TRUE);
    }
}