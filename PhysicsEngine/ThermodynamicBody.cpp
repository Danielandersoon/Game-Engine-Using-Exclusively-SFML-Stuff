#include "./ThermodynamicBody.h"
#include "./PhysicsWorld.h"

void GUESS::physics::ThermodynamicBody::updateTemperature(float deltaTime, PhysicsWorld* pWorld) {
    // Newton's Law of Cooling
    // dT/dt = k(Tambient - T)
    // Simple Euler integration
    float k = conductivity / (specificHeatCapacity * surfaceArea);
    float ambientTemp = pWorld->getAmbientTemp();
    temperature += k * (ambientTemp - temperature) * deltaTime;
}

void GUESS::physics::ThermodynamicBody::transferHeat(ThermodynamicBody* other) {
    // Fourier's Law of Heat Transfer
    // Q = k * A * (T1 - T2) * dt
    float heatTransferCoeff = (conductivity + other->getConductivity()) / 2.0f;
    float contactArea = std::min(surfaceArea, other->getSurfaceArea());
    float tempDiff = temperature - other->getTemperature();

    // Note: This should be called with deltaTime from the physics world
    // For now using a small fixed timestep as approximation
    float deltaTime = 1.0f / 60.0f;
    float heatTransferred = heatTransferCoeff * contactArea * tempDiff * deltaTime;

    // Update temperatures based on heat transfer
    // Heat capacity = mass * specific heat capacity (not surface area)
    // Assuming mass is proportional to volume, approximate from surface area
    float myHeatCapacity = specificHeatCapacity * surfaceArea;
    float otherHeatCapacity = other->getHeatCapacity() * other->getSurfaceArea();

    float myTempChange = heatTransferred / myHeatCapacity;
    float otherTempChange = heatTransferred / otherHeatCapacity;

    temperature -= myTempChange;
    other->setTemperature(other->getTemperature() + otherTempChange);
}

float GUESS::physics::ThermodynamicBody::getTemperature() const {
    return temperature;
}

void GUESS::physics::ThermodynamicBody::setTemperature(float temp) {
    temperature = temp;
}
