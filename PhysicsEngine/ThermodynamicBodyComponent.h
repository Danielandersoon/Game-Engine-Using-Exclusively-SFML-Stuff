#ifndef THERMODYNAMIC_BODY_COMPONENT_H
#define THERMODYNAMIC_BODY_COMPONENT_H
#include "../GMath.h"
#include "./ThermodynamicBody.h"
#include "../Component.h"

namespace GUESS::core {
    class ThermodynamicBodyComponent : public Component {
    public:
        float temperature{ 293.15f }; // Default 20°C in Kelvin
        float specificHeatCapacity{ 1000.0f }; // J/kg·K
        float conductivity{ 0.5f }; // W/m·K
        float surfaceArea{ 1.0f }; // m²

        std::unique_ptr<GUESS::physics::ThermodynamicBody> thermodynamicBody;

    public:
        void init() override {
            setName("thermodynamic body component");
            thermodynamicBody = std::make_unique<GUESS::physics::ThermodynamicBody>();

            thermodynamicBody.get()->setConductivity(conductivity);
            thermodynamicBody.get()->setHeatCapacity(specificHeatCapacity);
            thermodynamicBody.get()->setSurfaceArea(surfaceArea);
            thermodynamicBody.get()->setTemperature(temperature);
        };

        void update();
    };
}

#endif