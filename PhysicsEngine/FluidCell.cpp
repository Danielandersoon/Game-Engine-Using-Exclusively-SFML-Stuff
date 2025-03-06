#include "./FluidCell.h"

namespace GUESS::physics {
    void FluidCell::updateState() {
        if (temperature < thermalProperties->getMeltingPoint()) {
            state = solid;
        }
        else if (temperature > thermalProperties->getBoilingPoint()) {
            state = gas;
        }
        else {
            state = liquid;
        }
    }

    void FluidCell::updateVelocity(const GUESS::core::math::Vector2f& newVel) {
        if (state != solid) {
            // Apply viscosity damping
            float dampingFactor = 1.0f / (1.0f + properties.viscosity);
            velocity = velocity * dampingFactor + newVel;
        }
    }

    void FluidCell::calculatePressure(float gravity) {
        if (state != solid) {
            properties.pressure = properties.density * gravity * cellSize;
        }
    }

    void FluidCell::mixWith(FluidCell* other) {
        if (state == liquid && other->state == liquid) {
            // Calculate new properties based on mass-weighted averaging
            float totalMass = mass + other->mass;
            float ratio1 = mass / totalMass;
            float ratio2 = other->mass / totalMass;

            FluidProperties newProps;
            newProps.density = properties.density * ratio1 + other->properties.density * ratio2;
            newProps.viscosity = properties.viscosity * ratio1 + other->properties.viscosity * ratio2;
            newProps.pressure = (properties.pressure + other->properties.pressure) / 2.0f;

            properties = newProps;
            other->properties = newProps;
        }
    }

    void FluidCell::transferHeat(FluidCell* neighbor) {
        if (thermalProperties && neighbor->thermalProperties) {
            thermalProperties->transferHeat(neighbor->thermalProperties);
        }
    }
}
