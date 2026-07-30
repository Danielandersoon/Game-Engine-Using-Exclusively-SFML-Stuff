#ifndef PHYSICS_WORLD_H
#define PHYSICS_WORLD_H

#include <algorithm>
#include <unordered_map>
#include <vector>
#include <thread>
#include "./Collider.h"
#include "./EnvironmentManager.h"

namespace GUESS::physics {
    class PhysicsWorld {
    private:
        std::vector<RigidBody<GUESS::core::math::Vector3f>*> bodies3D;
        EnvironmentManager envManager;
        float fixedTimeStep;
        std::unordered_map<RigidBody<GUESS::core::math::Vector3f>*, int> restingFrames3D;

        static const size_t BATCH_SIZE = 64; 
        static constexpr int REST_FRAMES_TO_SLEEP = 8;

        inline static size_t NUM_THREADS;
        std::vector<std::thread> workerThreads;

        // processCollisionBatch3D will detect overlaps and append index pairs to an output vector (thread-safe via mutex)
        void processCollisionBatch3D(size_t start, size_t end, std::vector<RigidBody<GUESS::core::math::Vector3f>*>& bodies,
                                     std::vector<std::pair<size_t,size_t>>* outPairs, std::mutex* outMutex);

        void startPhysicsThreads();
        void stopPhysicsThreads();

    public:
        PhysicsWorld(float timeStep = 1.0f / 60.0f) : fixedTimeStep(timeStep) { 
            NUM_THREADS = std::thread::hardware_concurrency();
            if (NUM_THREADS == 0) NUM_THREADS = 1;
        }

        PhysicsWorld(PhysicsWorld&& other) noexcept {
            bodies3D = std::move(other.bodies3D);
            envManager = std::move(other.envManager);
            fixedTimeStep = other.fixedTimeStep;
            workerThreads = std::move(other.workerThreads);
        }

        PhysicsWorld& operator=(PhysicsWorld&& other) noexcept {
            if (this != &other) {
                bodies3D = std::move(other.bodies3D);
                envManager = std::move(other.envManager);
                fixedTimeStep = other.fixedTimeStep;
                workerThreads = std::move(other.workerThreads);
            }
            return *this;
        }

        PhysicsWorld(const PhysicsWorld&) = delete;
        PhysicsWorld& operator=(const PhysicsWorld&) = delete;


        void addBody(RigidBody<GUESS::core::math::Vector3f>* body) {
            if (!body) return;
            if (std::find(bodies3D.begin(), bodies3D.end(), body) != bodies3D.end()) return;
            bodies3D.push_back(body);
        }

        void removeBody(RigidBody<GUESS::core::math::Vector3f>* body);

        void step(float deltaTime);
        void checkCollisions();

        void resolveCollision(RigidBody<GUESS::core::math::Vector3f>* bodyA, RigidBody<GUESS::core::math::Vector3f>* bodyB);
        void resolveCollision(RigidBody<GUESS::core::math::Vector3f>* bodyA, RigidBody<GUESS::core::math::Vector3f>* bodyB, const GUESS::core::math::Vector3f& normal);

        float getAmbientTemp() const { return envManager.getAmbientTemp(); }

        void updateThermodynamics(float deltaTime);

        void processThermalBatch(size_t start, size_t end); 

    };
}

#endif
