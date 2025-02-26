#ifndef SKYBOX_H
#define SKYBOX_H
#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include "../GMath.h"

namespace GUESS::rendering {

    class Camera;

    enum class SkyboxType {
        Cubic,
        Panoramic,
        Gradient,
        Solid
    };

    class Skybox {
    private:
        SkyboxType type;
        sf::Texture cubeTextures[6];              // For cubic
        sf::Texture panoramicTexture;             // For panoramic
        sf::Color topColor;                       // For gradient
        sf::Color bottomColor;                    // For gradient
        sf::Color solidColor;                     // For solid

    public:
        void loadCubic(const std::string paths[6]);
        void loadPanoramic(const std::string& path);
        void setGradient(const sf::Color& top, const sf::Color& bottom);
        void setSolidColor(const sf::Color& color);
        void render(const Camera& camera, sf::RenderTarget& target);
    };
}
#endif
