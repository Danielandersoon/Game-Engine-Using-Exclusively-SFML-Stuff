#ifndef MESH_RENDERER_COMPONENT_H
#define MESH_RENDERER_COMPONENT_H

#include "./Component.h"
#include "./RenderingEngine/Mesh.h"
#include "./RenderingEngine/Material.h"
#include <memory>
#include <string>

namespace GUESS::core {
    class MeshRendererComponent : public Component {
    public:
        // Public attributes for Unity-like behavior
        std::string meshPath;
        std::shared_ptr<GUESS::rendering::threed::Mesh> mesh;
        std::shared_ptr<GUESS::rendering::Material> material;
        bool castShadows = true;
        bool receiveShadows = true;

        MeshRendererComponent() = default;

        void init() override {
            setName("mesh render componenet");
            if (!meshPath.empty()) {
                loadMesh(meshPath);
            }
        }

        void loadMesh(const std::string& path) {
            mesh = std::make_shared<GUESS::rendering::threed::Mesh>();

            // Get file extension
            size_t dotPos = path.find_last_of('.');
            if (dotPos != std::string::npos) {
                std::string extension = path.substr(dotPos);

                if (extension == ".obj") {
                    mesh->loadFromOBJ(path);
                }
                else if (extension == ".fbx") {
                    mesh->loadFromFBX(path);
                }
                else if (extension == ".blend") {
                    mesh->loadFromBlend(path);
                }
            }
            meshPath = path;
        }


        void setMaterial(std::shared_ptr<GUESS::rendering::Material> newMaterial) {
            material = newMaterial;
        }

        std::shared_ptr<GUESS::rendering::threed::Mesh> getMesh() { return mesh; };
        std::shared_ptr<GUESS::rendering::Material> getMaterial() { return material; }

    };
}

#endif
