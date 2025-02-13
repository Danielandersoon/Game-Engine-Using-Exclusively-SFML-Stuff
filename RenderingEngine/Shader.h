#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "../GMath.h"
#include "../Logger.h"

namespace GUESS::rendering {
    class Shader {
    private:
        sf::Shader shader;
        std::string vertexPath;
        std::string fragmentPath;

    public:
        Shader() = default;
        bool loadFromFile(const std::string& vertexPath, const std::string& fragmentPath);

        void setUniform(const std::string& name, float value);
        void setUniform(const std::string& name, int value);
        void setUniform(const std::string& name, const GUESS::core::math::Vector2f& value);
        void setUniform(const std::string& name, const GUESS::core::math::Vector3f& value);
        void setUniform(const std::string& name, const GUESS::core::math::Matrix4x4& value);
        void setUniform(const std::string& name, const sf::Texture& texture);

        void bind();
        void unbind();

        sf::Shader* getNativeShader() { return &shader; }
    };
}
#endif
