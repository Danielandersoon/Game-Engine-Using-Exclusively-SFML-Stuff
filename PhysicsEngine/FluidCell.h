#ifndef FLUID_CELL_H
#define FLUID_CELL_H
#include "../GMath.h"
#include "./ThermodynamicBody.h"

namespace GUESS::physics {
    enum FluidState { gas, liquid, solid };

    struct FluidProperties {
        float density;      // kg/m³
        float viscosity;    // Pa·s
        float pressure;     // Pa
    };

    class FluidCell {
    private:
        FluidProperties properties;
        float temperature;
        GUESS::core::math::Vector2f velocity;
        FluidState state;
        ThermodynamicBody* thermalProperties;
        float mass;
        float cellSize;  // Size of cell in meters

    public:
        FluidCell(float size = 1.0f) :
            temperature(293.15f),
            velocity(0.0f, 0.0f),
            state(liquid),
            cellSize(size) {
            properties = { 1000.0f, 1.0f, 0.0f }; // Default to water properties
            mass = properties.density * (cellSize * cellSize);
        }

        void updateState();
        void updateVelocity(const GUESS::core::math::Vector2f& newVel);
        void transferHeat(FluidCell* neighbor);
        void calculatePressure(float gravity);
        void mixWith(FluidCell* other);

        const FluidProperties& getProperties() const { return properties; }
        void setProperties(const FluidProperties& props) {
            properties = props;
            mass = properties.density * (cellSize * cellSize);
        }
        float getMass() const { return mass; }
        float getCellSize() const { return cellSize; }
        float getTemperature() const { return temperature; }
        GUESS::core::math::Vector2f getVelocity() const { return velocity; }
        FluidState getState() const { return state; }
    };
};
#endif
