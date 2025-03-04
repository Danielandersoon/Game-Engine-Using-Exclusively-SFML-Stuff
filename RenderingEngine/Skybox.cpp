#include "./Skybox.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "./Camera.h"

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
            // Create a cube mesh centered at camera position
            sf::RectangleShape faces[6];
            const float size = 1000.0f; // Large size to encompass scene

            // Configure faces
            for (int i = 0; i < 6; i++) {
                faces[i].setSize(sf::Vector2f(size, size));
                faces[i].setTexture(&cubeTextures[i]);
            }

            // Position faces relative to camera
            faces[0].setPosition(camera.getPosition().x - size / 2, camera.getPosition().y - size / 2); // Front
            faces[1].setPosition(camera.getPosition().x + size / 2, camera.getPosition().y - size / 2); // Back
            faces[2].setPosition(camera.getPosition().x - size / 2, camera.getPosition().y - size / 2); // Left
            faces[3].setPosition(camera.getPosition().x - size / 2, camera.getPosition().y + size / 2); // Right
            faces[4].setPosition(camera.getPosition().x - size / 2, camera.getPosition().y - size / 2); // Top
            faces[5].setPosition(camera.getPosition().x - size / 2, camera.getPosition().y + size / 2); // Bottom

            // Draw faces in order based on camera rotation
            for (const auto& face : faces) {
                target.draw(face);
            }
            break;
        }
        case SkyboxType::Panoramic: {
            // Create spherical projection
            sf::RectangleShape panorama;
            const float radius = 1000.0f;

            panorama.setSize(sf::Vector2f(radius * 2.0f * GUESS::core::math::PI, radius));
            panorama.setTexture(&panoramicTexture);

            // Center at camera
            panorama.setPosition(
                camera.getPosition().x - radius * GUESS::core::math::PI,
                camera.getPosition().y - radius / 2
            );

            // Calculate UV coordinates based on camera view direction
            float u = camera.getRotation().toEuler().y / (2.0f * GUESS::core::math::PI);
            panorama.setTextureRect(sf::IntRect(
                u * panoramicTexture.getSize().x,
                0,
                panoramicTexture.getSize().x,
                panoramicTexture.getSize().y
            ));

            target.draw(panorama);
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
