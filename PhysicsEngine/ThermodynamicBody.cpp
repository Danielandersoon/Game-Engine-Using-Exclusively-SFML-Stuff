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
    // Q = k * A * (T1 - T2)
    float heatTransferCoeff = (conductivity + other->getConductivity()) / 2.0f;
    float contactArea = std::min(surfaceArea, other->getSurfaceArea());
    float tempDiff = temperature - other->getTemperature();

    float heatTransferred = heatTransferCoeff * contactArea * tempDiff;

    // Update temperatures based on heat transfer
    float myTempChange = heatTransferred / (specificHeatCapacity * surfaceArea);
    float otherTempChange = heatTransferred / (other->getHeatCapacity() * other->getSurfaceArea());

    temperature -= myTempChange;
    other->setTemperature(other->getTemperature() + otherTempChange);
}

float GUESS::physics::ThermodynamicBody::getTemperature() const {
    return temperature;
}

void GUESS::physics::ThermodynamicBody::setTemperature(float temp) {
    temperature = temp;
}
