#ifndef PHYSICS_WORLD_H
#define PHYSICS_WORLD_H

#include <vector>
#include "./Collider.h"

namespace GUESS::physics {
    class PhysicsWorld {
    private:
        std::vector<RigidBody<GUESS::core::math::Vector2f>*> bodies2D;
        std::vector<RigidBody<GUESS::core::math::Vector3f>*> bodies3D;
        float fixedTimeStep;

    public:
        PhysicsWorld(float timeStep = 1.0f / 60.0f) : fixedTimeStep(timeStep) {}

        void addBody(RigidBody<GUESS::core::math::Vector2f>* body) { bodies2D.push_back(body); }
        void addBody(RigidBody<GUESS::core::math::Vector3f>* body) { bodies3D.push_back(body); }

        void removeBody(RigidBody<GUESS::core::math::Vector2f>* body);
        void removeBody(RigidBody<GUESS::core::math::Vector3f>* body);

        void step(float deltaTime);
        void checkCollisions();
    };
}

#endif
