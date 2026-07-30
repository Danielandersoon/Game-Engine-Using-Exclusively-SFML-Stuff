#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "../GMath.h"
#include "../Logger.h"
#include "Light.h"

namespace GUESS::rendering {
    class Shader {
    private:
        sf::Shader shader;
        std::string vertexPath;
        std::string fragmentPath;

        GUESS::core::math::Vector3f position;
        GUESS::core::math::Vector3f color;
        float intensity;

        threed::LightType type;

        bool m_isValid = false;

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

        void setSpecular(float strength);
        void setShininess(float value);
        void setAmbient(const GUESS::core::math::Vector3f& ambient);

        void setupInstancing();
        const sf::Shader* getNativeShader() const { return &shader; }

        bool isValid() const { return m_isValid; }
    };
}
#endif
