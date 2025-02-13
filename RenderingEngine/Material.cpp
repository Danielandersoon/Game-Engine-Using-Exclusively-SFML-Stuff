#include "Material.h"

namespace GUESS::rendering {

    Material::Material() : color(1.0f, 1.0f, 1.0f) {
    }

    void Material::setShader(const std::string& vertexShader, const std::string& fragmentShader) {
        if (!shader.loadFromFile(vertexShader, fragmentShader)) {
            GUESS::core::Logger::log(GUESS::core::Logger::ERROR, "Failed to load shader files");
        }
    }

    bool Material::setMainTexture(const std::string& texturePath) {
        mainTexture = std::make_unique<sf::Texture>();
        if (!mainTexture->loadFromFile(texturePath)) {
            GUESS::core::Logger::log(GUESS::core::Logger::ERROR, "Failed to load texture: " + texturePath);
            mainTexture.reset();
            return false;
        }
        shader.setUniform("mainTexture", *mainTexture);
        return true;
    }

    void Material::setColor(const GUESS::core::math::Vector3f& newColor) {
        color = newColor;
        shader.setUniform("materialColor", sf::Glsl::Vec3(color.x, color.y, color.z));
    }

    void Material::bind() {
        if (mainTexture) {
            shader.setUniform("mainTexture", *mainTexture);
        }
        shader.setUniform("materialColor", sf::Glsl::Vec3(color.x, color.y, color.z));
    }

    sf::Shader* Material::getShader() {
        return &shader;
    }

    sf::Texture* Material::getMainTexture() const {
        return mainTexture.get();
    }

    const GUESS::core::math::Vector3f& Material::getColor() const {
        return color;
    }
}
