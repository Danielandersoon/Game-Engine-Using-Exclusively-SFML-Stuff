#ifndef MATERIAL_H
#define MATERIAL_H
#include <SFML/Graphics.hpp>
#include "../GMath.h"
#include "Shader.h"

namespace GUESS::rendering {
    class Material {
    private:
        std::unique_ptr<sf::Texture> mainTexture;
        GUESS::core::math::Vector3f color;
        std::unique_ptr<sf::Texture> lightMap;
        bool hasLightMap = false;

    protected:
        GUESS::rendering::Shader shader;

    public:
        Material();
        void setShader(const std::string& vertexShader, const std::string& fragmentShader);
        bool setMainTexture(const std::string& texturePath);
        void setColor(const GUESS::core::math::Vector3f& color);
        void bind();

        GUESS::rendering::Shader& getShader() ;
        sf::Texture* getMainTexture() const;
        const GUESS::core::math::Vector3f& getColor() const;

        void setLightMap(const std::string& texturePath);
        void setLightMap(const sf::RenderTexture& lmTecxture);

        bool hasLightmap() const { return hasLightMap; }
    };
}
#endif
