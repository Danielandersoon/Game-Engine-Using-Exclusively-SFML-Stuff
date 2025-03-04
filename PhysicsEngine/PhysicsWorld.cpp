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

}
