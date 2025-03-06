#ifndef THERMODYNAMIC_BODY_H
#define THERMODYNAMIC_BODY_H

#include "../Component.h"

    namespace GUESS::physics {

        class PhysicsWorld;

        struct ThermalMaterial {
            float density;
            float specificHeat;
            float thermalConductivity;
            float meltingPoint;
            float boilingPoint;
        };

        class ThermodynamicBody {
        private:
            // We're going metric bitches
            float temperature{ 293.15f }; // Default 20°C in Kelvin
            float specificHeatCapacity{ 1000.0f }; // J/kg·K
            float conductivity{ 0.5f }; // W/m·K
            float surfaceArea{ 1.0f }; // m²

        public:
            ThermodynamicBody() = default;
            ThermodynamicBody(float temp, float capacity, float conduct, float area)
                : temperature(temp), specificHeatCapacity(capacity), conductivity(conduct), surfaceArea(area) {}

            void updateTemperature(float deltaTime, PhysicsWorld* pWorld);
            void transferHeat(ThermodynamicBody* other);
            float getTemperature() const;
            void setTemperature(float temp);

            // Add getters/setters for other properties
            float getHeatCapacity() const { return specificHeatCapacity; }
            float getConductivity() const { return conductivity; }
            float getSurfaceArea() const { return surfaceArea; }

            void setHeatCapacity(float capacity) { specificHeatCapacity = capacity; }
            void setConductivity(float conduct) { conductivity = conduct; }
            void setSurfaceArea(float area) { surfaceArea = area; }
        };
    }

#endif
