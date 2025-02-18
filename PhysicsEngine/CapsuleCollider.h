#ifndef CAPSULE_COLLIDER_H
#define CAPSULE_COLLIDER_H

#include "Collider.h"

namespace GUESS::physics {
    class CapsuleCollider : public Collider<GUESS::core::math::Vector3f> {
    public:
        CapsuleCollider(float radius = 1.0f, float height = 2.0f);

        bool checkCollision(const Collider<GUESS::core::math::Vector3f>& other) override;

        void setRadius(float r) { radius = r; }
        void setHeight(float h) { height = h; }

        float getRadius() const { return radius; }
        float getHeight() const { return height; }

    private:
        float radius;
        float height;
    };
}

#endif
