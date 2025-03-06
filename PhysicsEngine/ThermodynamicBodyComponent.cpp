#include "./ThermodynamicBodyComponent.h"
#include "../Scene.h"

void GUESS::core::ThermodynamicBodyComponent::update()
{
    thermodynamicBody.get()->updateTemperature(m_ownerScene.get()->FixedDeltaTime(), m_ownerScene.get()->getPhysicsWorld());

    temperature = thermodynamicBody.get()->getTemperature();
    specificHeatCapacity = thermodynamicBody.get()->getHeatCapacity();
    conductivity = thermodynamicBody.get()->getConductivity();
    surfaceArea = thermodynamicBody.get()->getConductivity();
}
