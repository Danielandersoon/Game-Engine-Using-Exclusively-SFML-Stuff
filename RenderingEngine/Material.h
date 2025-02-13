#ifndef MATERIAL_H
#define MATERIAL_H
#include <SFML/Graphics.hpp>
#include "../GMath.h"
#include "Shader.h"

namespace GUESS::rendering {
    class Material {
    private:
        sf::Shader shader;
        std::unique_ptr<sf::Texture> mainTexture;
        GUESS::core::math::Vector3f color;

    public:
        Material();
        void setShader(const std::string& vertexShader, const std::string& fragmentShader);
        bool setMainTexture(const std::string& texturePath);
        void setColor(const GUESS::core::math::Vector3f& color);
        void bind();

        sf::Shader* getShader();
        sf::Texture* getMainTexture() const;
        const GUESS::core::math::Vector3f& getColor() const;
    };
}
#endif
