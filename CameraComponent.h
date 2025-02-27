#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include "./Component.h"
#include "./RenderingEngine/Camera.h"
#include "./GameObject.h"
#include <memory>

namespace GUESS::core {
    class CameraComponent : public Component {
    public:
        // Public attributes for Unity-like behavior
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
            // Update camera position/rotation based on GameObject transform
            camera.setPosition(getOwner()->getTransform().getPosition());
            camera.setRotation(getOwner()->getTransform().getRotation());
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
