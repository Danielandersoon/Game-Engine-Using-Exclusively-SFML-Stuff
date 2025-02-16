#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H
#include <vector>
#include "Shader.h"
#include "Light.h"

namespace GUESS::rendering::threed
{
    class LightManager {
    private:
        std::vector<Light> lights;
    public:
        void addLight(const Light& light) { lights.push_back(light); }
        void updateLightUniforms(Shader& shader);
    };

} 

#endif