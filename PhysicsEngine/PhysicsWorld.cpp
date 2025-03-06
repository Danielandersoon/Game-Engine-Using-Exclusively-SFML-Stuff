#include "./PhysicsWorld.h"
#include "./PhysicsConst.h"
#include "./PhysicsManager.h"
#include "./RigidBody.h"
#include "./RigidBody.h"
#include "./Rigidbody2D.h"
#include "../Logger.h"

namespace GUESS::physics {
    void PhysicsWorld::removeBody(RigidBody<GUESS::core::math::Vector2f>* body) {
        bodies2D.erase(
            std::remove(bodies2D.begin(), bodies2D.end(), body),
            bodies2D.end()
        );
        GUESS::core::Logger::log(GUESS::core::Logger::INFO, "Removed 2D rigidbody from physics world");
    }

    void PhysicsWorld::removeBody(RigidBody<GUESS::core::math::Vector3f>* body) {
        bodies3D.erase(
            std::remove(bodies3D.begin(), bodies3D.end(), body),
            bodies3D.end()
        );
        GUESS::core::Logger::log(GUESS::core::Logger::INFO, "Removed 3D rigidbody from physics world");
    }

    void PhysicsWorld::step(float deltaTime) {
        // Update 2D physics bodies
        for (auto* body : bodies2D) {
            body->update(fixedTimeStep);
        }

        // Update 3D physics bodies
        for (auto* body : bodies3D) {
            body->update(fixedTimeStep);
        }

        checkCollisions();
    }

    void PhysicsWorld::checkCollisions() {
        size_t batchSize2D = bodies2D.size() / NUM_THREADS;
        size_t batchSize3D = bodies3D.size() / NUM_THREADS;

        std::vector<std::future<void>> futures;

        // Launch 2D collision detection threads
        for (size_t i = 0; i < NUM_THREADS; i++) {
            size_t start = i * batchSize2D;
            size_t end = (i == NUM_THREADS - 1) ? bodies2D.size() : (i + 1) * batchSize2D;

            futures.push_back(std::async(std::launch::async,
                &PhysicsWorld::processCollisionBatch, this, start, end, std::ref(bodies2D)));
        }

        // Launch 3D collision detection threads
        for (size_t i = 0; i < NUM_THREADS; i++) {
            size_t start = i * batchSize3D;
            size_t end = (i == NUM_THREADS - 1) ? bodies3D.size() : (i + 1) * batchSize3D;

            futures.push_back(std::async(std::launch::async,
                &PhysicsWorld::processCollisionBatch3D, this, start, end, std::ref(bodies3D)));
        }

        // Wait for all threads to complete
        for (auto& future : futures) {
            future.wait();
        }
    }

    void PhysicsWorld::resolveCollision(RigidBody<GUESS::core::math::Vector2f>* bodyA, RigidBody<GUESS::core::math::Vector2f>* bodyB) {
        // Calculate relative velocity
        auto relativeVel = bodyB->getVelocity() - bodyA->getVelocity();

        // Calculate combined restitution and friction
        float restitution = std::min(bodyA->getRestitution(), bodyB->getRestitution());
        float friction = GUESS::core::math::sqrt(bodyA->getFriction() * bodyB->getFriction());

        // Apply impulse
        float relativeVelMagnitude = relativeVel.magnitude();
        auto normalizedVel = relativeVel.normalized();
        float impulseMag = -(1.0f + restitution) * relativeVelMagnitude;
        impulseMag /= (1.0f / bodyA->getMass() + 1.0f / bodyB->getMass());
        auto impulse = normalizedVel * impulseMag;

        bodyA->addForce((impulse * -1.0) * (1.0f / bodyA->getMass()));
        bodyB->addForce(impulse * (1.0f / bodyB->getMass()));

        // Apply friction
        auto tangent = relativeVel - (relativeVel.normalized() * relativeVel.dot(relativeVel.normalized()));
        if (tangent.magnitude() > 0.0001f) {
            tangent = tangent.normalized();
            float frictionImpulse = -relativeVel.dot(tangent);
            frictionImpulse /= (1.0f / bodyA->getMass() + 1.0f / bodyB->getMass());
            frictionImpulse *= friction;

            bodyA->setVelocity(bodyA->getVelocity().x - tangent.x * frictionImpulse * (1.0f / bodyA->getMass()));
            bodyB->setVelocity(bodyB->getVelocity().x + tangent.x * frictionImpulse * (1.0f / bodyB->getMass()));
        }
    }

    void PhysicsWorld::resolveCollision(RigidBody<GUESS::core::math::Vector3f>* bodyA, RigidBody<GUESS::core::math::Vector3f>* bodyB) {
        // Calculate relative velocity
        auto relativeVel = bodyB->getVelocity() - bodyA->getVelocity();

        // Calculate combined restitution and friction
        float restitution = std::min(bodyA->getRestitution(), bodyB->getRestitution());
        float friction = GUESS::core::math::sqrt(bodyA->getFriction() * bodyB->getFriction());

        // Apply impulse
        float relativeVelMagnitude = relativeVel.magnitude();
        auto normalizedVel = relativeVel.normalized();
        float impulseMag = -(1.0f + restitution) * relativeVelMagnitude;
        impulseMag /= (1.0f / bodyA->getMass() + 1.0f / bodyB->getMass());
        auto impulse = normalizedVel * impulseMag;

        bodyA->addForce((impulse * -1.0) * (1.0f / bodyA->getMass()));
        bodyB->addForce(impulse * (1.0f / bodyB->getMass()));

        // Apply friction
        auto tangent = relativeVel - (relativeVel.normalized() * relativeVel.dot(relativeVel.normalized()));
        if (tangent.magnitude() > 0.0001f) {
            tangent = tangent.normalized();
            float frictionImpulse = -relativeVel.dot(tangent);
            frictionImpulse /= (1.0f / bodyA->getMass() + 1.0f / bodyB->getMass());
            frictionImpulse *= friction;

            bodyA->setVelocity(bodyA->getVelocity().x - tangent.x * frictionImpulse * (1.0f / bodyA->getMass()));
            bodyB->setVelocity(bodyB->getVelocity().x + tangent.x * frictionImpulse * (1.0f / bodyB->getMass()));
        }
    }

    void PhysicsWorld::updateThermodynamics(float deltaTime)
    {
        std::vector<std::shared_ptr<ThermodynamicBody>> thermalBodies = envManager.getThermalBodies();
        // Update all thermal bodies in one pass
        for (auto body : thermalBodies) {
            body->updateTemperature(deltaTime, this);
        }
        // Process heat transfer in batches
        for (size_t i = 0; i < thermalBodies.size(); i += BATCH_SIZE) {
            processThermalBatch(i, std::min(i + BATCH_SIZE, thermalBodies.size()));
        }
    }

    void PhysicsWorld::processThermalBatch(size_t start, size_t end) {
        std::vector<std::shared_ptr<ThermodynamicBody>> thermalBodies = envManager.getThermalBodies();
        for (size_t i = start; i < end; i++) {
            for (size_t j = i + 1; j < thermalBodies.size(); j++) {
                thermalBodies[i]->transferHeat(thermalBodies[j].get());
            }
        }
    }

    void PhysicsWorld::processCollisionBatch(size_t start, size_t end,
        std::vector<RigidBody<GUESS::core::math::Vector2f>*>& bodies) {
        static std::mutex localMutex;
        std::lock_guard<std::mutex> lock(localMutex);

        for (size_t i = start; i < end; i++) {
            for (size_t j = i + 1; j < bodies.size(); j++) {
                auto colliderA = bodies[i]->getCollider();
                auto colliderB = bodies[j]->getCollider();

                if (colliderA && colliderB && colliderA->checkCollision(*colliderB)) {
                    std::lock_guard<std::mutex> lock(localMutex);
                    colliderA->onCollisionEnter(*colliderB);
                    colliderB->onCollisionEnter(*colliderA);
                }
            }
        }
    }

    void PhysicsWorld::processCollisionBatch3D(size_t start, size_t end,
        std::vector<RigidBody<GUESS::core::math::Vector3f>*>& bodies) {
        static std::mutex localMutex;
        std::lock_guard<std::mutex> lock(localMutex);

        for (size_t i = start; i < end; i++) {
            for (size_t j = i + 1; j < bodies.size(); j++) {
                auto colliderA = bodies[i]->getCollider();
                auto colliderB = bodies[j]->getCollider();

                if (colliderA && colliderB && colliderA->checkCollision(*colliderB)) {
                    std::lock_guard<std::mutex> lock(localMutex);
                    colliderA->onCollisionEnter(*colliderB);
                    colliderB->onCollisionEnter(*colliderA);
                }
            }
        }
    }
}
