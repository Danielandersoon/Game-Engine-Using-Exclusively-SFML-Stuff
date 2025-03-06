#ifndef FLUID_SIMULATOR_H
#define FLUID_SIMULATOR_H
#include "./FluidGrid.h"

namespace GUESS::physics {
    class PhysicsWorld;

    class FluidSimulator {
    private:
        FluidGrid grid;
        PhysicsWorld* physicsWorld;

    public:
        FluidSimulator(size_t width, size_t height, PhysicsWorld* world)
            : grid(width, height), physicsWorld(world) {}

        void step(float deltaTime);
        void propagateHeat();
    };
}
#endif
