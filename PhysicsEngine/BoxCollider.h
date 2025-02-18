#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include "Collider.h"

namespace GUESS::physics {
    class BoxCollider : public Collider<GUESS::core::math::Vector3f> {
    public:
        BoxCollider(const GUESS::core::math::Vector3f& dimensions = GUESS::core::math::Vector3f(1.0f, 1.0f, 1.0f));
        bool checkCollision(const Collider<GUESS::core::math::Vector3f>& other) override;
        void onCollisionEnter(Collider<GUESS::core::math::Vector3f>& other) override;
        void onCollisionStay(Collider<GUESS::core::math::Vector3f>& other) override;
        void onCollisionExit(Collider<GUESS::core::math::Vector3f>& other) override;

    private:
        void resolveCollision(Collider<GUESS::core::math::Vector3f>& other);
    };
};

#endif // !BOX_COLLIDER_H
