#include "./Camera.h"

namespace GUESS::rendering {
    Camera::Camera(float height, float width, float far, float near, float fov)
        : m_height(height), m_width(width), m_far(far), m_near(near), FOV(fov),
        m_aspectRatio(height != 0.0f ? width / height : 1.0f),
        position(0.0f, 0.0f), rotation(1.0f, 0.0f, 0.0f, 0.0f) {

        setProjectionMatrix(m_aspectRatio);

        // Initialize SFML view - center it at the middle of the view area
        m_view.setSize(m_width, m_height);
        m_view.setCenter(m_width / 2.0f, m_height / 2.0f);
    }

    Camera& Camera::operator=(const Camera& other)
    {
        if (this != &other) {
            m_height = other.m_height;
            m_width = other.m_width;
            m_far = other.m_far;
            m_near = other.m_near;
            FOV = other.FOV;
            m_aspectRatio = other.m_aspectRatio;
            position = other.position;
            rotation = other.rotation;
            m_view = other.m_view;
            m_frustum = other.m_frustum;
        }
        return *this;
    }

    Camera::Camera(const Camera& other) noexcept
        : m_height(other.m_height),
        m_width(other.m_width),
        m_far(other.m_far),
        m_near(other.m_near),
        FOV(other.FOV),
        m_aspectRatio(other.m_aspectRatio),
        m_frustum(other.m_frustum),
        position(other.position),
        rotation(other.rotation),
        m_view(other.m_view)
    {
        // Deep copy the skybox if it exists
        if (other.skybox) {
            skybox = std::make_unique<Skybox>(*other.skybox);
        }
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

    void Camera::setViewportSize(float width, float height) {
        if (width <= 0.0f || height <= 0.0f) {
            return;
        }

        m_width = width;
        m_height = height;
        m_aspectRatio = m_width / m_height;
        m_view.setSize(m_width, m_height);
        setProjectionMatrix(m_aspectRatio);
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
        m_aspectRatio = aspectRatio;
        GUESS::core::math::Matrix4x4 projectionMatrix = GUESS::core::math::Matrix4x4::perspective(
            GUESS::core::math::toRadians(FOV),
            m_aspectRatio,
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
    
    GUESS::core::math::Matrix4x4 Camera::getViewMatrix() const {
        GUESS::core::math::Matrix4x4 viewMatrix;
        viewMatrix = GUESS::core::math::Matrix4x4::translation(-position.x, -position.y, -position.z) *
            GUESS::core::math::Matrix4x4::rotation(rotation.x, rotation.y, rotation.z, rotation.w);
        return viewMatrix;
    }

    GUESS::core::math::Matrix4x4 Camera::getProjectionMatrix() const {
        return GUESS::core::math::Matrix4x4::perspective(
            GUESS::core::math::toRadians(FOV),
            m_aspectRatio,
            m_near,
            m_far
        );
    }

    bool Camera::isInFrustum(const GUESS::core::math::AABB& bounds, const GUESS::core::math::Matrix4x4& worldMatrix) const {
        // Transform AABB by world matrix
        GUESS::core::math::AABB transformedBounds = bounds.transform(worldMatrix);
        return m_frustum.isAABBVisible(transformedBounds);
    }

}
