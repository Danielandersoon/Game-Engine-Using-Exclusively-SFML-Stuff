#ifndef LIGHT_H
#define LIGHT_H
#include <SFML/Graphics.hpp>
#include "../GMath.h"

namespace GUESS::rendering::threed {
    enum class LightType { Point, Directional, Spot };
    class Light {
    private:
        GUESS::core::math::Vector3f position;
        GUESS::core::math::Vector3f colour = GUESS::core::math::Vector3f(255.f, 255.f, 255.f);
        GUESS::core::math::Quaternion rotation;
        float intensity = 1;
        float range = 10;
        LightType type;
        GUESS::core::math::Matrix4x4 m_lightSpaceMatrix;
        sf::Texture m_shadowMap;

    public:
        Light();
        Light(LightType type);
        void setPosition(const GUESS::core::math::Vector3f& pos) { position = pos; }
        void setColor(const GUESS::core::math::Vector3f& col) { colour = col; }
        void setIntensity(float i) { intensity = i; }
        void setType(LightType t) { type = t; }
        const GUESS::core::math::Matrix4x4& getLightSpaceMatrix() const { return m_lightSpaceMatrix; }
        const sf::Texture& getShadowMap() const { return m_shadowMap; }
        void setShadowMap(const sf::Texture& shadowMap);
        void setLightSpaceMatrix(const GUESS::core::math::Matrix4x4& matrix);
        void setForward(const GUESS::core::math::Vector3f& forward);
        void setRotation(GUESS::core::math::Quaternion q_in) { rotation = q_in; }
        GUESS::core::math::Quaternion getRotation() { return rotation; }
        void setRange(float r_in) { range = r_in; }
        float getRange() { return range; }

        const GUESS::core::math::Vector3f& getPosition() const { return position; }
        const GUESS::core::math::Vector3f& getColor() const { return colour; }
        float getIntensity() const { return intensity; }
        LightType getType() const { return type; }
        GUESS::core::math::Vector3f getForward() const{
            GUESS::core::math::Vector3f forward(0, 0, 1);
            GUESS::core::math::Matrix4x4 rotMatrix = rotation.toMatrix();
            return rotMatrix * forward;
        }
    };
}

#endif