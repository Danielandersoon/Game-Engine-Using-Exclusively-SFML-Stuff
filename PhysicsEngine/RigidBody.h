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

        float submergedVolume = 0.0f;
        float fluidDensity = 1000.0f; // Default water density in kg/m^3
        float volume = 1.0f;

        bool isStatic = false;
        bool sleeping = false;

    public:
        RigidBody() = default;
        explicit RigidBody(float mass, float friction = 0.2f, float restitution = 0.8f, Collider<T>* collider_ptr_in = nullptr)
            : mass(mass), friction(friction), restitution(restitution), collider_ptr(collider_ptr_in) {};

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
                return;
            }

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

            // Clear forces for next frame
            clearForces();

            // Reset acceleration
            acceleration = T();
        }
    };

    using Rigidbody3D = RigidBody<GUESS::core::math::Vector3f>;
};

#endif