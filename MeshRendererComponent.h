#ifndef MESH_RENDERER_COMPONENT_H
#define MESH_RENDERER_COMPONENT_H

#include "./Component.h"
#include "./RenderingEngine/Mesh.h"
#include "./RenderingEngine/Material.h"
#include "./Logger.h"
#include <filesystem>
#include <memory>
#include <string>

namespace GUESS::core {
    class MeshRendererComponent : public Component {
    public:
        std::string meshPath;
        std::shared_ptr<GUESS::rendering::threed::Mesh> mesh;
        std::shared_ptr<GUESS::rendering::Material> material;
        bool castShadows = true;
        bool receiveShadows = true;

        MeshRendererComponent() = default;

        void init() override {
            setName("mesh render component");
            if (!meshPath.empty()) {
                loadMesh(meshPath);
            }
        }

        void loadMesh(const std::string& path) {
            mesh = std::make_shared<GUESS::rendering::threed::Mesh>();
            std::string resolvedPath = path;

            // Resolve common runtime working-directory differences (e.g. x64/Debug).
            if (!std::filesystem::exists(resolvedPath)) {
                static const char* fallbackPrefixes[] = {
                    "./", "../", "../../", "../../../"
                };

                for (const char* prefix : fallbackPrefixes) {
                    std::string candidate = std::string(prefix) + path;
                    if (std::filesystem::exists(candidate)) {
                        resolvedPath = candidate;
                        break;
                    }
                }
            }

            bool loaded = false;
            size_t dotPos = path.find_last_of('.');
            if (dotPos != std::string::npos) {
                std::string extension = path.substr(dotPos);
                if (extension == ".obj") {
                    loaded = mesh->loadFromOBJ(resolvedPath);
                }
                else if (extension == ".fbx") {
                    loaded = mesh->loadFromFBX(resolvedPath);
                }
                else if (extension == ".blend") {
                    loaded = mesh->loadFromBlend(resolvedPath);
                }
            }

            if (!loaded || mesh->getVertices().empty() || mesh->getIndices().empty()) {
                GUESS::core::Logger::log(
                    GUESS::core::Logger::ERROR,
                    "Mesh load failed or produced empty geometry. Requested='" + path +
                    "', resolved='" + resolvedPath + "'"
                );
            }
            meshPath = path;
        }

        void setMaterial(std::shared_ptr<GUESS::rendering::Material> newMaterial) {
            material = newMaterial;
        }

        std::shared_ptr<GUESS::rendering::threed::Mesh> getMesh() { return mesh; }
        std::shared_ptr<GUESS::rendering::Material> getMaterial() { return material; }
    };
}
#endif
