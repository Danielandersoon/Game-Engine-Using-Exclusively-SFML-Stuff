#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <unordered_map>
#include <memory>
#include <string>
#include "./RenderingEngine/Shader.h"
#include "./RenderingEngine/Mesh.h"
#include "Logger.h"

namespace GUESS::core {

    class ResourceManager {
    private:
        std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;
        std::unordered_map<std::string, std::shared_ptr<GUESS::rendering::threed::Mesh>> meshes;
        std::unordered_map<std::string, std::shared_ptr<GUESS::rendering::Shader>> shaders;

        static ResourceManager* instance;

        ResourceManager() = default;

    public:
        static ResourceManager& getInstance() {
            if (!instance) {
                instance = new ResourceManager();
            }
            return *instance;
        }

        template<typename T>
        std::shared_ptr<T> load(const std::string& path);

        template<typename T>
        std::shared_ptr<T> get(const std::string& path);

        void unload(const std::string& path);
        void unloadAll();
    };

} // namespace GUESS::core
#endif
