#ifndef COLLIDER_H
#define COLLIDER_H

#include "../GMath.h"
#include "./RigidBody.h"

namespace GUESS::physics {
    template <typename T>
    class Collider {
    public:
        Collider() : isTrigger(false), attachedRigidbody(nullptr) {}

        virtual bool checkCollision(const Collider<T>& other) = 0;
        virtual void onCollisionEnter(Collider<T>& other) = 0;
        virtual void onCollisionStay(Collider<T>& other) = 0;
        virtual void onCollisionExit(Collider<T>& other) = 0;

        virtual T getCenter() const { return center; }
        T getDimensions() const { return dimensions; }
        bool getTrigger() const { return isTrigger; }
        RigidBody<T>* getAttachedRigidbody() { return attachedRigidbody; }
        T getScale() const { return scale; }
        float getRotation() const { return rotation; }
        GUESS::core::math::Quaternion getOrientation() const { return orientation; }

        void setDimensions(const T& dimensions) { this->dimensions = dimensions; }
        void setPosition(const T& position) {
            this->position = position;
            updateCenter();
        }
        void setTrigger(bool isTrigger) { this->isTrigger = isTrigger; }
        void setRotation(float angle) {
            this->rotation = angle;
            updateCenter();
        }
        void setOrientation(const GUESS::core::math::Quaternion& quat) {
            this->orientation = quat;
            updateCenter();
        }
        void setScale(const T& scale) {
            this->scale = scale;
            updateCenter();
        }
        void setAttachedRigidbody(RigidBody<T>* rb) { attachedRigidbody = rb; }

    protected:
        virtual void updateCenter() {
            center = position;  // Override in derived classes for more complex center calculations
        }

        bool isTrigger;
        float rotation;  // 2D rotation (radians)
        GUESS::core::math::Quaternion orientation = GUESS::core::math::Quaternion(1.0f, 0.0f, 0.0f, 0.0f);  // 3D rotation
        T dimensions;
        T position;
        T center;
        T scale;
        RigidBody<T>* attachedRigidbody;
    };
};

#endif
