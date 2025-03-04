#ifndef MATERIAL_H
#define MATERIAL_H
#include <SFML/Graphics.hpp>
#include "../GMath.h"
#include "./Shader.h"

namespace GUESS::rendering {
    class Material {
    private:
        std::unique_ptr<sf::Texture> mainTexture;
        GUESS::core::math::Vector3f color;
        std::unique_ptr<sf::Texture> lightMap;
        bool hasLightMap = false;
        bool isValid = false;

    protected:
        GUESS::rendering::Shader shader;

    public:
        Material();
        bool setShader(const std::string& vertexShader, const std::string& fragmentShader);
        bool setMainTexture(const std::string& texturePath);
        bool setColor(const GUESS::core::math::Vector3f& color);
        bool bind();
        bool isInitialized() const { return isValid; }
        bool getIsValid() const { return isValid; };

        GUESS::rendering::Shader& getShader() { return shader; };
        sf::Texture* getMainTexture() const { return mainTexture.get(); };
        const GUESS::rendering::Shader& getShader() const { return shader; }
        const GUESS::core::math::Vector3f& getColor() const { return color; };

        bool setLightMap(const std::string& texturePath);
        bool setLightMap(const sf::RenderTexture& lmTexture);

        bool hasLightmap() const { return hasLightMap; }
    };
}
#endif
