#include "FluidSimulationComponent.h"

#include <memory>
#include "./Scene.h"

void GUESS::core::FluidSimulationComponent::init()
{
    if (m_ownerScene) {
        fluidSim = std::make_unique<physics::FluidSimulator>(
            gridWidth,
            gridHeight,
            m_ownerScene->getPhysicsWorld()
        );
    }
}

void GUESS::core::FluidSimulationComponent::update()
{
    if (fluidSim) {
        fluidSim->step(m_ownerScene->DeltaTime());
    }
}

void GUESS::core::FluidSimulationComponent::fixedUpdate()
{
    if (fluidSim) {
        fluidSim->propagateHeat();
    }
}
