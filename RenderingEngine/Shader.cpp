#include "Shader.h"

namespace GUESS::rendering {
    bool Shader::loadFromFile(const std::string& vertPath, const std::string& fragPath) {
        vertexPath = vertPath;
        fragmentPath = fragPath;

        if (!shader.loadFromFile(vertexPath, fragmentPath)) {
            GUESS::core::Logger::log(GUESS::core::Logger::ERROR, "Failed to load shader files: " + vertexPath + ", " + fragmentPath);
            return false;
        }
        return true;
    }

    void Shader::setUniform(const std::string& name, float value) {
        shader.setUniform(name, value);
    }

    void Shader::setUniform(const std::string& name, int value) {
        shader.setUniform(name, value);
    }

    void Shader::setUniform(const std::string& name, const GUESS::core::math::Vector2f& value) {
        shader.setUniform(name, sf::Glsl::Vec2(value.x, value.y));
    }

    void Shader::setUniform(const std::string& name, const GUESS::core::math::Vector3f& value) {
        shader.setUniform(name, sf::Glsl::Vec3(value.x, value.y, value.z));
    }

    void Shader::setUniform(const std::string& name, const GUESS::core::math::Matrix4x4& value) {
        shader.setUniform(name, sf::Glsl::Mat4(value.m[0]));
    }

    void Shader::setUniform(const std::string& name, const sf::Texture& texture) {
        shader.setUniform(name, texture);
    }

    void Shader::bind() {
        sf::Shader::bind(&shader);
    }

    void Shader::unbind() {
        sf::Shader::bind(nullptr);
    }
}
