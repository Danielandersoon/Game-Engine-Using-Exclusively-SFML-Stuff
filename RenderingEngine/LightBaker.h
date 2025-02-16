#ifndef LIGHT_BAKER_H
#define LIGHT_BAKER_H
#include "Light.h"
#include "Mesh.h"
#include "Material.h"

namespace GUESS::rendering::threed
{
    class LightBaker {
    private:
        struct LightmapTexel {
            GUESS::core::math::Vector3f worldPosition;
            GUESS::core::math::Vector3f normal;
            GUESS::core::math::Vector3f irradiance;
            float occlusion;
        };

        std::vector<LightmapTexel> lightmapData;
        unsigned int resolution = 1024;
        sf::RenderTexture lightmapRenderTexture;

        public:
            void bakeStaticLights(const std::vector<Light>& lights, const std::vector<Mesh*>& staticMeshes);
            void applyLightmap(Material& material);
            sf::Texture getLightmapTexture() { return lightmapRenderTexture.getTexture(); }
            void generateAmbientOcclusion(const Mesh& mesh);

        private:
            void calculatePointLightContribution(LightmapTexel& texel, const Light& light);
            void calculateDirectionalLightContribution(LightmapTexel& texel, const Light& light);
            void calculateSpotLightContribution(LightmapTexel& texel, const Light& light);

    };
}
    
#endif