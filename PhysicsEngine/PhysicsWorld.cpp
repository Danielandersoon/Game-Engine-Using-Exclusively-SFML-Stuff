#include "PhysicsWorld.h"
#include "PhysicsConst.h"
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
        // Check 2D collisions
        for (size_t i = 0; i < bodies2D.size(); i++) {
            for (size_t j = i + 1; j < bodies2D.size(); j++) {
                auto colliderA = bodies2D[i]->getCollider();
                auto colliderB = bodies2D[j]->getCollider();

                if (colliderA && colliderB &&
                    colliderA->checkCollision(*colliderB)) {
                    colliderA->onCollisionEnter(*colliderB);
                    colliderB->onCollisionEnter(*colliderA);
                }
            }
        }

        // Check 3D collisions
        for (size_t i = 0; i < bodies3D.size(); i++) {
            for (size_t j = i + 1; j < bodies3D.size(); j++) {
                auto colliderA = bodies3D[i]->getCollider();
                auto colliderB = bodies3D[j]->getCollider();

                if (colliderA && colliderB &&
                    colliderA->checkCollision(*colliderB)) {
                    colliderA->onCollisionEnter(*colliderB);
                    colliderB->onCollisionEnter(*colliderA);
                }
            }
        }
    }
}
