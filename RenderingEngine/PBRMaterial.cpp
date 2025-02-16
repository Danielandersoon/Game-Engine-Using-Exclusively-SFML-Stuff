#include "PBRMaterial.h"

namespace GUESS::rendering {
    void PBRMaterial::setRoughness(float value) {
        roughness = value;
        shader.setUniform("roughness", roughness);
    }
    
    void PBRMaterial::setMetallic(float value) {
        metallic = value;
        shader.setUniform("metallic", metallic);
    }
    
    void PBRMaterial::setNormalMap(const std::string& path) {
        normalMap = std::make_unique<sf::Texture>();
        if (normalMap->loadFromFile(path)) {
            shader.setUniform("normalMap", *normalMap);
        }
    }
    
    void PBRMaterial::bind() {
        Material::bind();
        shader.setUniform("roughness", roughness);
        shader.setUniform("metallic", metallic);
        shader.setUniform("ao", ambientOcclusion);
        
        if (normalMap) shader.setUniform("normalMap", *normalMap);
        if (roughnessMap) shader.setUniform("roughnessMap", *roughnessMap);
        if (metallicMap) shader.setUniform("metallicMap", *metallicMap);
    }
    
}