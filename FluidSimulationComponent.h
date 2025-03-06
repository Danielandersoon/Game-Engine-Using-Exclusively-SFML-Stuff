#ifndef FLUID_SIMULATION_COMPONENT_H
#define FLUID_SIMULATION_COMPONENT_H

#include "./Component.h"
#include "./PhysicsEngine/FluidSimulator.h"

namespace GUESS::core {
    class FluidSimulationComponent : public Component {
    private:
        std::unique_ptr<physics::FluidSimulator> fluidSim;
        size_t gridWidth;
        size_t gridHeight;
        float resolution;

    public:
        FluidSimulationComponent(size_t width = 64, size_t height = 64, float res = 1.0f)
            : gridWidth(width), gridHeight(height), resolution(res) {}

        void init() override;

        void update();

        void fixedUpdate();

        physics::FluidSimulator* getSimulator() { return fluidSim.get(); }
    };
}

#endif
