#ifndef ENVIRONMENT_MANAGER_H
#define ENVIRONMENT_MANAGER_H

#include <vector>
#include "./ThermodynamicBody.h"

namespace GUESS::physics {
    class EnvironmentManager {
    private:
        float ambientTemperature;
        float pressure;
        float humidity;
        std::vector<std::shared_ptr<ThermodynamicBody>> thermalBodies;
        
    public:
        float getAmbientTemp() const { return ambientTemperature; }
        float getPreassure() const { return pressure; }
        float getHumidity() const { return humidity; }

        std::vector<std::shared_ptr<ThermodynamicBody>> getThermalBodies() const { return thermalBodies; };

        void updateEnvironment(float deltaTime);
        void addThermalBody(ThermodynamicBody* body);
        void setAmbientTemperature(float temp);
        };
}

#endif
