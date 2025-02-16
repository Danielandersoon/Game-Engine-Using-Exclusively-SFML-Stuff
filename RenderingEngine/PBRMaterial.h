#ifndef PBRMATERIAL_H
#define PBRMATERIAL_H
#include "Material.h"
#include "Shader.h"
namespace GUESS::rendering {
    class PBRMaterial : public Material {
    private:
        float roughness = 0.5f;
        float metallic = 0.0f;
        float ambientOcclusion = 1.0f;
        std::unique_ptr<sf::Texture> normalMap;
        std::unique_ptr<sf::Texture> roughnessMap;
        std::unique_ptr<sf::Texture> metallicMap;

    public:
        void initialize();
        void setRoughness(float value);
        void setMetallic(float value);
        void setNormalMap(const std::string& path);
        void bind();
        void setAmbientOcclusion(float value);
        void setRoughnessMap(const std::string& path);
        void setMetallicMap(const std::string& path);
    };
}
    
#endif