#include "./Material.h"

namespace GUESS::rendering {

    Material::Material() : color(1.0f, 1.0f, 1.0f), isValid(false) {
    }

    bool Material::setShader(const std::string& vertexShader, const std::string& fragmentShader) {
        if (vertexShader.empty() || fragmentShader.empty()) {
            GUESS::core::Logger::log(GUESS::core::Logger::ERROR, "Invalid shader file paths");
            return false;
        }

        if (!shader.loadFromFile(vertexShader, fragmentShader)) {
            GUESS::core::Logger::log(GUESS::core::Logger::ERROR, "Failed to load shader files: " + vertexShader + ", " + fragmentShader);
            return false;
        }

        isValid = true;
        return true;
    }

    bool Material::setMainTexture(const std::string& texturePath) {
        if (texturePath.empty()) {
            GUESS::core::Logger::log(GUESS::core::Logger::ERROR, "Invalid texture path");
            return false;
        }

        mainTexture = std::make_unique<sf::Texture>();
        if (!mainTexture->loadFromFile(texturePath)) {
            GUESS::core::Logger::log(GUESS::core::Logger::ERROR, "Failed to load texture: " + texturePath);
            mainTexture.reset();
            return false;
        }

        if (!shader.isValid()) {
            GUESS::core::Logger::log(GUESS::core::Logger::WARNING, "Shader not initialized when setting texture");
            return false;
        }

        shader.setUniform("mainTexture", *mainTexture);
        return true;
    }

    bool Material::setColor(const GUESS::core::math::Vector3f& newColor) {
        if (!shader.isValid()) {
            GUESS::core::Logger::log(GUESS::core::Logger::ERROR, "Shader not initialized when setting color");
            return false;
        }

        color = newColor;
        shader.setUniform("materialColor", color);
        return true;
    }

    bool Material::bind() {
        if (!isValid || !shader.isValid()) {
            GUESS::core::Logger::log(GUESS::core::Logger::ERROR, "Cannot bind invalid material");
            return false;
        }

        if (mainTexture) {
            shader.setUniform("mainTexture", *mainTexture);
        }

        if (lightMap && hasLightMap) {
            shader.setUniform("lightMap", *lightMap);
            shader.setUniform("hasLightMap", true);
        }
        else {
            shader.setUniform("hasLightMap", false);
        }

        shader.setUniform("materialColor", color);
        return true;
    }

    bool Material::setLightMap(const std::string& texturePath) {
        if (texturePath.empty()) {
            GUESS::core::Logger::log(GUESS::core::Logger::ERROR, "Invalid lightmap path");
            return false;
        }

        lightMap = std::make_unique<sf::Texture>();
        if (!lightMap->loadFromFile(texturePath)) {
            GUESS::core::Logger::log(GUESS::core::Logger::ERROR, "Failed to load lightmap: " + texturePath);
            lightMap.reset();
            hasLightMap = false;
            return false;
        }

        hasLightMap = true;
        shader.setUniform("lightMap", *lightMap);
        return true;
    }

    bool Material::setLightMap(const sf::RenderTexture& lmTexture) {
        if (!lmTexture.getSize().x || !lmTexture.getSize().y) {
            GUESS::core::Logger::log(GUESS::core::Logger::ERROR, "Invalid render texture dimensions");
            return false;
        }

        lightMap = std::make_unique<sf::Texture>(lmTexture.getTexture());
        hasLightMap = true;
        shader.setUniform("lightMap", *lightMap);
        return true;
    }
}
