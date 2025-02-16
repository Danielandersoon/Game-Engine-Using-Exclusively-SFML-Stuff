#include "ResourceManager.h"

namespace GUESS::core {

    ResourceManager* ResourceManager::instance = nullptr;

    template<>
    std::shared_ptr<sf::Texture> ResourceManager::load(const std::string& path) {
        if (textures.find(path) != textures.end()) {
            return textures[path];
        }

        auto texture = std::make_shared<sf::Texture>();
        if (!texture->loadFromFile(path)) {
            Logger::log(Logger::ERROR, "Failed to load texture: " + path);
            return nullptr;
        }

        textures[path] = texture;
        return texture;
    }

    template<>
    std::shared_ptr<rendering::threed::Mesh> ResourceManager::load(const std::string& path) {
        if (meshes.find(path) != meshes.end()) {
            return meshes[path];
        }

        auto mesh = std::make_shared<rendering::threed::Mesh>();
        if (!mesh->loadFromOBJ(path)) {
            Logger::log(Logger::ERROR, "Failed to load mesh: " + path);
            return nullptr;
        }

        meshes[path] = mesh;
        return mesh;
    }

    template<>
    std::shared_ptr<rendering::Shader> ResourceManager::load(const std::string& path) {
        if (shaders.find(path) != shaders.end()) {
            return shaders[path];
        }

        auto shader = std::make_shared<rendering::Shader>();
        if (!shader->loadFromFile(path + ".vert", path + ".frag")) {
            Logger::log(Logger::ERROR, "Failed to load shader: " + path);
            return nullptr;
        }

        shaders[path] = shader;
        return shader;
    }

    template<typename T>
    std::shared_ptr<T> ResourceManager::get(const std::string& path) {
        if constexpr (std::is_same_v<T, sf::Texture>) {
            return textures[path];
        }
        else if constexpr (std::is_same_v<T, rendering::threed::Mesh>) {
            return meshes[path];
        }
        else if constexpr (std::is_same_v<T, rendering::Shader>) {
            return shaders[path];
        }
        return nullptr;
    }

    void ResourceManager::unload(const std::string& path) {
        textures.erase(path);
        meshes.erase(path);
        shaders.erase(path);
        Logger::log(Logger::INFO, "Unloaded resource: " + path);
    }

    void ResourceManager::unloadAll() {
        textures.clear();
        meshes.clear();
        shaders.clear();
        Logger::log(Logger::INFO, "Unloaded all resources");
    }

} // namespace GUESS::core
