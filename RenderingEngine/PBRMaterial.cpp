#include "PBRMaterial.h"
#include "../Logger.h"

namespace GUESS::rendering {

    void PBRMaterial::bind() {
        Material::bind();  

        // Bind PBR specific parameters
        shader.setUniform("material.roughness", roughness);
        shader.setUniform("material.metallic", metallic);
        shader.setUniform("material.ao", ambientOcclusion);

        // Bind PBR textures if they exist
        if (normalMap) {
            shader.setUniform("material.normalMap", *normalMap);
            shader.setUniform("material.hasNormalMap", true);
        }
        else {
            shader.setUniform("material.hasNormalMap", false);
        }

        if (roughnessMap) {
            shader.setUniform("material.roughnessMap", *roughnessMap);
            shader.setUniform("material.hasRoughnessMap", true);
        }
        else {
            shader.setUniform("material.hasRoughnessMap", false);
        }

        if (metallicMap) {
            shader.setUniform("material.metallicMap", *metallicMap);
            shader.setUniform("material.hasMetallicMap", true);
        }
        else {
            shader.setUniform("material.hasMetallicMap", false);
        }
    }

    void PBRMaterial::setRoughness(float value) {
        roughness = std::clamp(value, 0.0f, 1.0f);
    }

    void PBRMaterial::setMetallic(float value) {
        metallic = std::clamp(value, 0.0f, 1.0f);
    }

    void PBRMaterial::setAmbientOcclusion(float value) {
        ambientOcclusion = std::clamp(value, 0.0f, 1.0f);
    }

    void PBRMaterial::setNormalMap(const std::string& path) {
        normalMap = std::make_unique<sf::Texture>();
        if (!normalMap->loadFromFile(path)) {
            GUESS::core::Logger::log(GUESS::core::Logger::ERROR, "Failed to load normal map: " + path);
            normalMap.reset();
        }
    }

    void PBRMaterial::setRoughnessMap(const std::string& path) {
        roughnessMap = std::make_unique<sf::Texture>();
        if (!roughnessMap->loadFromFile(path)) {
            GUESS::core::Logger::log(GUESS::core::Logger::ERROR, "Failed to load roughness map: " + path);
            roughnessMap.reset();
        }
    }

    void PBRMaterial::setMetallicMap(const std::string& path) {
        metallicMap = std::make_unique<sf::Texture>();
        if (!metallicMap->loadFromFile(path)) {
            GUESS::core::Logger::log(GUESS::core::Logger::ERROR, "Failed to load metallic map: " + path);
            metallicMap.reset();
        }
    }

    void PBRMaterial::initialize() {
        // Load PBR shader
        setShader("shaders/pbr.vert", "shaders/pbr.frag");

        // Set default values
        setRoughness(0.5f);
        setMetallic(0.0f);
        setAmbientOcclusion(1.0f);

        // Set default PBR parameters in shader
        shader.setUniform("material.albedo", GUESS::core::math::Vector3f(1.0f, 1.0f, 1.0f));
        shader.setUniform("material.F0", GUESS::core::math::Vector3f(0.04f, 0.04f, 0.04f));
    }
}
