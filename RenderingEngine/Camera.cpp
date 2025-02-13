#include "Camera.h"

namespace GUESS::rendering {
    Camera::Camera(float height, float width, float far, float near, float fov)
        : m_height(height), m_width(width), m_far(far), m_near(near), FOV(fov),
        position(0.0f, 0.0f), rotation(1.0f, 0.0f, 0.0f, 0.0f) {

        // Create perspective projection matrix for the frustum
        GUESS::core::math::Matrix4x4 projectionMatrix =
            GUESS::core::math::Matrix4x4::perspective(
                GUESS::core::math::toRadians(FOV),
                m_width / m_height,
                m_near,
                m_far
            );

        // Update the frustum planes based on the projection matrix
        m_frustum.updateFromMatrix(projectionMatrix);

        // Initialize SFML view
        m_view.setSize(m_width, m_height);
        m_view.setCenter(0.0f, 0.0f);
    }

    void Camera::setPosition(const GUESS::core::math::Vector3f& newPos) {
        position = newPos;
        updateView();
    }

    void Camera::setRotation(float angle) {
        rotation = GUESS::core::math::Quaternion::fromEuler(0.0f, 0.0f, angle);
        updateView();
    }

    void Camera::setRotation(const GUESS::core::math::Quaternion& newRotation) {
        rotation = newRotation;
        updateView();
    }

    void Camera::move(const GUESS::core::math::Vector3f& offset) {
        position = position + offset;
        updateView();
    }

    void Camera::rotate(float angle) {
        rotation = rotation * GUESS::core::math::Quaternion::fromEuler(0.0f, 0.0f, angle);
        updateView();
    }

    void Camera::zoom(float factor) {
        m_view.zoom(factor);
        updateView();
    }

    void Camera::updateView() {
        m_view.setCenter(position.x, position.y);
        m_view.setRotation(GUESS::core::math::toDegrees(
            2.0f * GUESS::core::math::arccos(rotation.w)
        ));
    }

    bool Camera::isVisible(const GUESS::core::math::Vector3f& point) const {
        return m_frustum.isPointVisible(GUESS::core::math::Vector3f(point.x, point.y, 0.0f));
    }

    void Camera::setProjectionMatrix(float aspectRatio) {
        GUESS::core::math::Matrix4x4 projectionMatrix = GUESS::core::math::Matrix4x4::perspective(
            GUESS::core::math::toRadians(FOV),
            aspectRatio,
            m_near,
            m_far
        );
        m_frustum.updateFromMatrix(projectionMatrix);
    }

    void Camera::setOrthographic(float left, float right, float bottom, float top) {
        m_view.setSize(right - left, top - bottom);
        m_view.setCenter((left + right) / 2.0f, (bottom + top) / 2.0f);
        updateView();
    }

    bool Camera::isInView(const GUESS::core::math::AABB& boundingBox) const {
        return m_frustum.isAABBVisible(boundingBox);
    }

    bool Camera::isInView(const GUESS::core::math::Vector3f& point, float radius) const {
        return m_frustum.isSphereVisible(point, radius);
    }

    void Camera::lookAt(const GUESS::core::math::Vector3f& target) {
        GUESS::core::math::Vector3f direction = (target - position).normalized();
        float angle = GUESS::core::math::toDegrees(GUESS::core::math::arctan(direction.y / direction.x));
        setRotation(angle);
    }

}
