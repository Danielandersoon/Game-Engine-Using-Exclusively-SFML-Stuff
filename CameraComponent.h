#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H
#include <memory>

#include "./Component.h"
#include "./RenderingEngine/Camera.h"
#include "./GameObject.h"
#include "./Scene.h"

namespace GUESS::core {
    class CameraComponent : public Component {
    public:
        float fieldOfView = 60.0f;
        float nearPlane = 0.1f;
        float farPlane = 1000.0f;
        bool isMainCamera = false;
        GUESS::rendering::Camera camera;

        CameraComponent() : camera(900.0f, 1440.0f, farPlane, nearPlane, fieldOfView) {
            setName("camera component");
        }

        void init() override {
            updateProjection();
        }

        void update() {
            auto owner = m_ownerScene->FindGameObject(getOwner());
            camera.setPosition(owner->getTransform().getPosition());
            camera.setRotation(owner->getTransform().getRotation());
        }

        void setFieldOfView(float fov) {
            fieldOfView = fov;
            updateProjection();
        }

        void setNearPlane(float near) {
            nearPlane = near;
            updateProjection();
        }

        void setFarPlane(float far) {
            farPlane = far;
            updateProjection();
        }

        GUESS::rendering::Camera& getCamera() { return camera; }

    private:
        void updateProjection() {
            float aspectRatio = camera.getSize().x / camera.getSize().y;
            camera.setProjectionMatrix(aspectRatio);
        }
    };
}
#endif
