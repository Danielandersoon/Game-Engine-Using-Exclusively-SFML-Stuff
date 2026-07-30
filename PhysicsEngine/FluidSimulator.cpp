#include "./FluidSimulator.h"

namespace GUESS::physics {
    void FluidSimulator::step(float deltaTime) {
        grid.calculatePressures();
        grid.updateCells();
        grid.handleFluidMixing();
        grid.updateThermodynamics();
        grid.handleStateTransitions();
        // TODO: Use deltaTime for time-based simulation updates
    }

    void FluidSimulator::propagateHeat() {
        grid.updateThermodynamics();
    }
}
