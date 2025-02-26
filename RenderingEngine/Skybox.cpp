#include "Skybox.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace GUESS::rendering {
    void Skybox::loadCubic(const std::string paths[6]) {
        type = SkyboxType::Cubic;
        for (int i = 0; i < 6; i++) {
            cubeTextures[i].loadFromFile(paths[i]);
        }
    }

    void Skybox::loadPanoramic(const std::string& path) {
        type = SkyboxType::Panoramic;
        panoramicTexture.loadFromFile(path);
    }

    void Skybox::setGradient(const sf::Color& top, const sf::Color& bottom) {
        type = SkyboxType::Gradient;
        topColor = top;
        bottomColor = bottom;
    }

    void Skybox::setSolidColor(const sf::Color& color) {
        type = SkyboxType::Solid;
        solidColor = color;
    }

    void Skybox::render(const Camera& camera, sf::RenderTarget& target) {
        switch (type) {
            case SkyboxType::Cubic: {
                // Render cube faces based on camera orientation
                break;
            }
            case SkyboxType::Panoramic: {
                // Render panoramic texture mapped to sphere
                break;
            }
            case SkyboxType::Gradient: {
                sf::RectangleShape sky(sf::Vector2f(target.getSize()));
                sky.setFillColor(topColor);
                
                sf::RectangleShape ground(sf::Vector2f(target.getSize().x, target.getSize().y/2.f));
                ground.setPosition(0, target.getSize().y/2.f);
                ground.setFillColor(bottomColor);
                
                target.draw(sky);
                target.draw(ground);
                break;
            }
            case SkyboxType::Solid: {
                sf::RectangleShape background(sf::Vector2f(target.getSize()));
                background.setFillColor(solidColor);
                target.draw(background);
                break;
            }
        }
    }
}
