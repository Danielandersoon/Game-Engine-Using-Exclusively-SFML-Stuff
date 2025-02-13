#ifndef CAMERA_H
#define CAMERA_H
#include <SFML/Graphics/View.hpp>
#include "../GMath.h"

namespace GUESS::rendering {
    class Camera {
    private:
        float m_height, m_width;
        float m_far, m_near;
        float FOV; // Degrees

        GUESS::core::math::Frustum m_frustum;
        GUESS::core::math::Vector3f position;
        GUESS::core::math::Quaternion rotation;
        sf::View m_view;

    public:
        Camera(float height, float width, float far = 1000.0f, float near = 0.1f, float fov = 60.0f);

        void setPosition(const GUESS::core::math::Vector3f& newPos);
        void setRotation(float angle);
        void setRotation(const GUESS::core::math::Quaternion& newRotation);
        void move(const GUESS::core::math::Vector3f& offset);
        void rotate(float angle);
        void zoom(float factor);

        const GUESS::core::math::Vector3f& getPosition() const { return position; }
        const GUESS::core::math::Quaternion& getRotation() const { return rotation; }
        const sf::View& getView() const { return m_view; }

        void updateView();
        bool isVisible(const GUESS::core::math::Vector3f& point) const;

        void lookAt(const GUESS::core::math::Vector3f& target);
        void setProjectionMatrix(float aspectRatio);
        void setOrthographic(float left, float right, float bottom, float top);

        bool isInView(const GUESS::core::math::AABB& boundingBox) const;
        bool isInView(const GUESS::core::math::Vector3f& point, float radius) const;
    };
}
#endif
