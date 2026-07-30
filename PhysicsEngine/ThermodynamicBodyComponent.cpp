#include "./ThermodynamicBodyComponent.h"
#include "../Scene.h"

void GUESS::core::ThermodynamicBodyComponent::update()
{
    thermodynamicBody->updateTemperature(m_ownerScene->FixedDeltaTime(), m_ownerScene->getPhysicsWorld());

    temperature = thermodynamicBody.get()->getTemperature();
    specificHeatCapacity = thermodynamicBody.get()->getHeatCapacity();
    conductivity = thermodynamicBody.get()->getConductivity();
    surfaceArea = thermodynamicBody.get()->getConductivity();
}
