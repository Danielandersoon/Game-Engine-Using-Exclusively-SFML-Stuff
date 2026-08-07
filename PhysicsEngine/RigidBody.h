#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../GMath.h"
#include "./PhysicsConst.h"

namespace GUESS::physics {
    template <typename T>
    class Collider;

    template <typename T>
    class RigidBody {
    protected:
        T position = T();
        T velocity = T();
        T acceleration = T();
        float mass = 1.0f;
        float friction = 0.2f;
        float restitution = 0.8f;
        Collider<T>* collider_ptr;
        T forceAccum = T();

        // Rotational properties (3D only - initialized in constructor)
        GUESS::core::math::Quaternion orientation;
        T angularVelocity;
        T angularAcceleration;
        T torqueAccum;
        T inertiaTensor;
        T inverseInertiaTensor;

        float submergedVolume = 0.0f;
        float fluidDensity = 1000.0f; // Default water density in kg/m^3
        float volume = 1.0f;

        bool isStatic = false;
        bool sleeping = false;

    public:
        RigidBody() {
            orientation = GUESS::core::math::Quaternion(1.0f, 0.0f, 0.0f, 0.0f); // Identity
            angularVelocity = T();
            angularAcceleration = T();
            torqueAccum = T();
            // Initialize inertia tensors for 3D
            if constexpr (std::is_same_v<T, GUESS::core::math::Vector3f>) {
                inertiaTensor = T(1.0f, 1.0f, 1.0f);
                inverseInertiaTensor = T(1.0f, 1.0f, 1.0f);
            } else {
                inertiaTensor = T();
                inverseInertiaTensor = T();
            }
        }
        explicit RigidBody(float mass, float friction = 0.2f, float restitution = 0.8f, Collider<T>* collider_ptr_in = nullptr)
            : mass(mass), friction(friction), restitution(restitution), collider_ptr(collider_ptr_in) {
            orientation = GUESS::core::math::Quaternion(1.0f, 0.0f, 0.0f, 0.0f); // Identity
            angularVelocity = T();
            angularAcceleration = T();
            torqueAccum = T();
            // Initialize inertia tensors for 3D
            if constexpr (std::is_same_v<T, GUESS::core::math::Vector3f>) {
                inertiaTensor = T(1.0f, 1.0f, 1.0f);
                inverseInertiaTensor = T(1.0f, 1.0f, 1.0f);
            } else {
                inertiaTensor = T();
                inverseInertiaTensor = T();
            }
        }

        void applyForce(const T& force) {
            sleeping = false;
            acceleration = acceleration + force;
        };
        T getPosition() const { return position; };
        T  getVelocity() const { return velocity; };
        T getAcceleration() const { return acceleration; };
        void setPosition(const T& position) { this->position = position; };
        void setVelocity(T velocity) {
            this->velocity = velocity;
            if (velocity.magnitude() > 0.0001f) {
                sleeping = false;
            }
        };
        void setAcceleration(const T& acceleration) { this->acceleration = acceleration; };
        float getMass() const { return mass; };
        float getFriction() const { return friction; };
        float getRestitution() const { return restitution; };
        void setMass(float mass) { this->mass = mass; };
        void setFriction(float friction) { this->friction = friction; };
        void setRestitution(float  restitution) { this->restitution = restitution; };
        Collider<T>* getCollider() { return collider_ptr; }
        void setCollider(Collider<T>* col) { collider_ptr = col; }
        void clearForces() { forceAccum = T(); }
        void addForce(const T& force) {
            sleeping = false;
            forceAccum = forceAccum + force;
        }
        void setSubmergedVolume(float volume) { submergedVolume = volume; }
        void setFluidDensity(float density) { fluidDensity = density; }
        void setVolume(float vol) { volume = vol; }

        // Rotational accessors (3D only)
        GUESS::core::math::Quaternion getOrientation() const { return orientation; }
        T getAngularVelocity() const { return angularVelocity; }
        T getInertiaTensor() const { return inertiaTensor; }
        T getInverseInertiaTensor() const { return inverseInertiaTensor; }

        void setOrientation(const GUESS::core::math::Quaternion& quat) { 
            orientation = quat; 
            orientation.normalize();
        }
        void setAngularVelocity(const T& angVel) { 
            angularVelocity = angVel; 
            if (angVel.magnitude() > 0.0001f) {
                sleeping = false;
            }
        }

        // Calculate inertia tensor for a box (call this when collider dimensions change)
        void calculateBoxInertia(const T& dimensions) {
            if constexpr (std::is_same_v<T, GUESS::core::math::Vector3f>) {
                float m = mass;
                float w = dimensions.x;
                float h = dimensions.y;
                float d = dimensions.z;

                // Inertia tensor for a box: I = (1/12) * m * (w^2 + h^2, w^2 + d^2, h^2 + d^2)
                inertiaTensor.x = (1.0f / 12.0f) * m * (h * h + d * d);
                inertiaTensor.y = (1.0f / 12.0f) * m * (w * w + d * d);
                inertiaTensor.z = (1.0f / 12.0f) * m * (w * w + h * h);

                // Calculate inverse (for diagonal tensor, just invert each component)
                inverseInertiaTensor.x = (inertiaTensor.x > 0.0f) ? (1.0f / inertiaTensor.x) : 0.0f;
                inverseInertiaTensor.y = (inertiaTensor.y > 0.0f) ? (1.0f / inertiaTensor.y) : 0.0f;
                inverseInertiaTensor.z = (inertiaTensor.z > 0.0f) ? (1.0f / inertiaTensor.z) : 0.0f;
            }
        }

        // Add torque (3D only)
        void addTorque(const T& torque) {
            if constexpr (std::is_same_v<T, GUESS::core::math::Vector3f>) {
                sleeping = false;
                torqueAccum = torqueAccum + torque;
            }
        }

        void clearTorques() {
            if constexpr (std::is_same_v<T, GUESS::core::math::Vector3f>) {
                torqueAccum = T();
            }
        }

        // Add force at a point (generates torque)
        void addForceAtPoint(const T& force, const T& point) {
            if constexpr (std::is_same_v<T, GUESS::core::math::Vector3f>) {
                addForce(force);
                T r = point - position; // Vector from center of mass to application point
                T torque = r.cross(force); // Torque = r × F
                addTorque(torque);
            } else {
                // 2D fallback
                addForce(force);
            }
        }

        // Static flag accessors
        void setIsStatic(bool s) { isStatic = s; }
        bool getIsStatic() const { return isStatic; }
        void setSleeping(bool s) {
            if (isStatic) {
                sleeping = false;
                return;
            }

            sleeping = s;
            if (sleeping) {
                velocity = T();
                acceleration = T();
                clearForces();
                if constexpr (std::is_same_v<T, GUESS::core::math::Vector3f>) {
                    angularVelocity = T();
                    angularAcceleration = T();
                    clearTorques();
                }
            }
        }
        bool getSleeping() const { return sleeping; }

        void calculateBuoyancy() {
            if (submergedVolume > 0.0f) {
                // Archimedes principle: Buoyant force = fluid density * submerged volume * gravity
                float buoyantForce = fluidDensity * submergedVolume * GRAVITY;

                // The force acts upward
                T buoyancyForce;
                if constexpr (std::is_same_v<T, GUESS::core::math::Vector2f>) {
                    buoyancyForce = T(0.0f, buoyantForce);
                }
                else {
                    buoyancyForce = T(0.0f, buoyantForce, 0.0f);
                }

                addForce(buoyancyForce);
            }
        }


        void update(float deltaTime) {
            if (isStatic || sleeping) {
                // Static bodies do not integrate
                clearForces();
                acceleration = T();
                if (isStatic) {
                    velocity = T();
                }
                if constexpr (std::is_same_v<T, GUESS::core::math::Vector3f>) {
                    clearTorques();
                    angularAcceleration = T();
                    if (isStatic) {
                        angularVelocity = T();
                    }
                }
                return;
            }

            // Linear motion integration
            acceleration = acceleration + (forceAccum * (1.0f / mass));

            calculateBuoyancy();
            // Apply gravity (F = m * g)
            T gravityForce;
            if constexpr (std::is_same_v<T, GUESS::core::math::Vector2f>) {
                gravityForce = T(0.0f, mass * GRAVITY);
            }
            else {
                gravityForce = T(0.0f, mass * GRAVITY, 0.0f);
            }
            acceleration = acceleration + (gravityForce * (1.0f / mass));

            velocity = velocity + (acceleration * deltaTime);
            position = position + (velocity * deltaTime);

            // Rotational motion integration (3D only)
            if constexpr (std::is_same_v<T, GUESS::core::math::Vector3f>) {
                // Angular acceleration = Inverse Inertia Tensor * Torque
                angularAcceleration.x = inverseInertiaTensor.x * torqueAccum.x;
                angularAcceleration.y = inverseInertiaTensor.y * torqueAccum.y;
                angularAcceleration.z = inverseInertiaTensor.z * torqueAccum.z;

                // Update angular velocity
                angularVelocity = angularVelocity + (angularAcceleration * deltaTime);

                // Apply angular damping (air resistance) - stronger to prevent wild spinning
                float angularDamping = 0.92f;
                angularVelocity = angularVelocity * angularDamping;

                // Update orientation using angular velocity
                // dq/dt = 0.5 * ω * q (quaternion derivative)
                if (angularVelocity.magnitude() > 0.0001f) {
                    GUESS::core::math::Quaternion angVelQuat(angularVelocity.x, angularVelocity.y, angularVelocity.z, 0.0f);
                    GUESS::core::math::Quaternion orientationDelta = angVelQuat * orientation;
                    orientationDelta.x *= 0.5f * deltaTime;
                    orientationDelta.y *= 0.5f * deltaTime;
                    orientationDelta.z *= 0.5f * deltaTime;
                    orientationDelta.w *= 0.5f * deltaTime;

                    orientation.x += orientationDelta.x;
                    orientation.y += orientationDelta.y;
                    orientation.z += orientationDelta.z;
                    orientation.w += orientationDelta.w;
                    orientation.normalize();
                }

                clearTorques();
                angularAcceleration = T();
            }

            // Clear forces for next frame
            clearForces();

            // Reset acceleration
            acceleration = T();
        }
    };

    using Rigidbody3D = RigidBody<GUESS::core::math::Vector3f>;
};

#endif