#ifndef LIGHT_COMPONENT_H
#define LIGHT_COMPONENT_H
#include "./Component.h"
#include "./RenderingEngine/Light.h"
#include "./Scene.h"


namespace GUESS::core {
	class LightComponent : public Component {
	private:
        std::unique_ptr <GUESS::rendering::threed::Light> light;

    public:
        GUESS::core::math::Vector3f position;
        GUESS::core::math::Vector3f colour = GUESS::core::math::Vector3f(255.f, 255.f, 255.f);
        GUESS::core::math::Quaternion rotation;
        float intensity = 1;
        float range = 10;
        GUESS::rendering::threed::LightType type;
        GUESS::core::math::Matrix4x4 lightSpaceMatrix;
    public:
        void init() override {
            setName("light component");
            light = std::make_unique<GUESS::rendering::threed::Light>();

            auto owner = m_ownerScene->FindGameObject(getOwner());
        }

        void update() {
            auto owner = m_ownerScene->FindGameObject(getOwner());
            if (!owner || !light) {
                return;
            }
            light.get()->setPosition(owner->getTransform().getPosition() + position);
            light.get()->setRotation(owner->getTransform().getRotation());
            light.get()->setColor(colour);
            light.get()->setIntensity(intensity);
            light.get()->setType(type);
            light.get()->setRange(range);

        }

        GUESS::rendering::threed::Light* getLight() const { return light.get(); }
	};

}

#endif // !LIGHT_COMPONENT_H
