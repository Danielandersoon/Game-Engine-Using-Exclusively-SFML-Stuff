#include "./Shader.h"

namespace GUESS::rendering {
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
        float glslMatrix[16];
        // Convert matrix elements one by one
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                glslMatrix[i * 4 + j] = value.m[i][j];
            }
        }
        shader.setUniform(name, sf::Glsl::Mat4(glslMatrix));
    }

    void Shader::setUniform(const std::string& name, const sf::Texture& texture) {
        shader.setUniform(name, texture);
    }

    bool Shader::loadFromFile(const std::string& vertPath, const std::string& fragPath) {
        if (vertPath.empty() || fragPath.empty()) {
            GUESS::core::Logger::log(GUESS::core::Logger::ERROR, "Empty shader path provided");
            m_isValid = false;
            return false;
        }

        vertexPath = vertPath;
        fragmentPath = fragPath;

        if (!shader.loadFromFile(vertexPath, fragmentPath)) {
            GUESS::core::Logger::log(GUESS::core::Logger::ERROR, "Failed to load shader files: " + vertexPath + ", " + fragmentPath);
            m_isValid = false;
            return false;
        }

        m_isValid = true;
        return true;
    }

    void Shader::bind() {
        if (m_isValid) {
            sf::Shader::bind(&shader);
        }
        else {
            GUESS::core::Logger::log(GUESS::core::Logger::WARNING, "Attempting to bind invalid shader");
        }
    }

    void Shader::unbind() {
        sf::Shader::bind(nullptr);
    }

    void Shader::setupInstancing() {
        // Add instance matrix attribute
        const std::string instancedVertexShader = R"(
        attribute mat4 instanceMatrix;
        uniform mat4 projectionMatrix;
        uniform mat4 viewMatrix;
        
        void main() {
            // Transform vertex position by instance matrix first, then view and projection
            vec4 worldPos = instanceMatrix * vec4(position, 1.0);
            vec4 viewPos = viewMatrix * worldPos;
            gl_Position = projectionMatrix * viewPos;
            
            // Transform normal by instance matrix
            vec3 worldNormal = mat3(instanceMatrix) * normal;
            
            // Pass transformed attributes to fragment shader
            vNormal = normalize(worldNormal);
            vUV = texCoord;
            vWorldPos = worldPos.xyz;
        }
    )";

        shader.loadFromMemory(instancedVertexShader, sf::Shader::Vertex);
    }

}
