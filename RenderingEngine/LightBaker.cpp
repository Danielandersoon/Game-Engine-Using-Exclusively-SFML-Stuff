#include "LightBaker.h"
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cmath>
#include "../GMath.h"

namespace GUESS::rendering::threed {
    void LightBaker::applyLightmap(Material& material) {
        material.setLightMap(lightmapRenderTexture);
        material.getShader().setUniform("hasLightMap", true);
    }

    void LightBaker::bakeStaticLights(const std::vector<Light>& lights, const std::vector<Mesh*>& staticMeshes) {
        lightmapRenderTexture.create(resolution, resolution);

        // Process each texel
        for (int y = 0; y < resolution; y++) {
            for (int x = 0; x < resolution; x++) {
                LightmapTexel& texel = lightmapData[y * resolution + x];
                texel.irradiance = GUESS::core::math::Vector3f(0, 0, 0);

                for (const auto& light : lights) {
                    switch (light.getType()) {
                    case Light::LightType::Point:
                        calculatePointLightContribution(texel, light);
                        break;
                    case Light::LightType::Directional:
                        calculateDirectionalLightContribution(texel, light);
                        break;
                    case Light::LightType::Spot:
                        calculateSpotLightContribution(texel, light);
                        break;
                    }
                }
            }
        }

        // Update lightmap texture
        sf::Image lightmapImage;
        lightmapImage.create(resolution, resolution);
        for (int y = 0; y < resolution; y++) {
            for (int x = 0; x < resolution; x++) {
                const auto& texel = lightmapData[y * resolution + x];
                lightmapImage.setPixel(x, y,
                    sf::Color(
                        texel.irradiance.x * 255,
                        texel.irradiance.y * 255,
                        texel.irradiance.z * 255
                    ));
            }
        }
        sf::Texture texture;
        texture.loadFromImage(lightmapImage);
        lightmapRenderTexture.clear();
        lightmapRenderTexture.draw(sf::Sprite(texture));
        lightmapRenderTexture.display();
    }


    void LightBaker::generateAmbientOcclusion(const Mesh& mesh) {
        const auto& vertices = mesh.getVertices();
        for (size_t i = 0; i < vertices.size(); i++) {
            float occlusion = 0.0f;
            const auto& vertex = vertices[i];

            GUESS::core::math::Random random;

            for (int ray = 0; ray < 32; ray++) {
                // Generate random direction in hemisphere using our math library
                GUESS::core::math::Vector3f rayDir = random.nextVector3();
                rayDir = rayDir.dot(vertex.normal) < 0 ? rayDir * -1.0f : rayDir;

                bool hit = false;
                float distance = 0.0f;

                if (hit) {
                    occlusion += 1.0f - (distance / 5.0f);
                }
            }

            lightmapData[i].occlusion = occlusion / 32.0f;
        }
    }

    void LightBaker::calculatePointLightContribution(LightmapTexel& texel, const Light& light) {
        GUESS::core::math::Vector3f lightDir = light.getPosition() - texel.worldPosition;
        float distance = lightDir.magnitude();
        lightDir = lightDir.normalized();

        // Calculate attenuation
        float attenuation = 1.0f / (1.0f + 0.09f * distance + 0.032f * distance * distance);

        // Calculate diffuse lighting
        float diff = std::max(0.0f, texel.normal.dot(lightDir));
        GUESS::core::math::Vector3f diffuse = light.getColor() * diff * attenuation * light.getIntensity();

        texel.irradiance = texel.irradiance + diffuse;
    }

    void LightBaker::calculateDirectionalLightContribution(LightmapTexel& texel, const Light& light) {
        GUESS::core::math::Vector3f lightDir = light.getForward().normalized() * -1.0f;

        // Calculate diffuse lighting
        float diff = std::max(0.0f, texel.normal.dot(lightDir));
        GUESS::core::math::Vector3f diffuse = light.getColor() * diff * light.getIntensity();

        texel.irradiance = texel.irradiance + diffuse;
    }

    void LightBaker::calculateSpotLightContribution(LightmapTexel& texel, const Light& light) {
        GUESS::core::math::Vector3f lightDir = light.getPosition() - texel.worldPosition;
        float distance = lightDir.magnitude();
        lightDir = lightDir.normalized();

        // Spot light parameters
        float cutOff = cos(GUESS::core::math::toRadians(12.5f));
        float outerCutOff = cos(GUESS::core::math::toRadians(17.5f));

        // Calculate spot light cone effect
        float theta = lightDir.dot(light.getForward() * -1.0f);
        float epsilon = cutOff - outerCutOff;
        float intensity = std::clamp((theta - outerCutOff) / epsilon, 0.0f, 1.0f);

        // Calculate attenuation
        float attenuation = 1.0f / (1.0f + 0.09f * distance + 0.032f * distance * distance);

        // Calculate diffuse lighting
        float diff = std::max(0.0f, texel.normal.dot(lightDir));
        GUESS::core::math::Vector3f diffuse = light.getColor() * diff * attenuation * intensity * light.getIntensity();

        texel.irradiance = texel.irradiance + diffuse;
    }
}
