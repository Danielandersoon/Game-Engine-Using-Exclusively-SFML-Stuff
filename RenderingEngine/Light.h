#ifndef LIGHT_H
#define LIGHT_H
#include <SFML/Graphics.hpp>
#include "../GMath.h"

namespace GUESS::rendering::threed {

    class Light {
    public:
        static const enum class LightType { Point, Directional, Spot };
    private:
        GUESS::core::math::Vector3f position;
        GUESS::core::math::Vector3f colour;
        GUESS::core::math::Quaternion rotation;
        float intensity;
        LightType type;
        GUESS::core::math::Matrix4x4 m_lightSpaceMatrix;
        sf::Texture m_shadowMap;

    public:
        void setPosition(const GUESS::core::math::Vector3f& pos) { position = pos; }
        void setColor(const GUESS::core::math::Vector3f& col) { colour = col; }
        void setIntensity(float i) { intensity = i; }
        void setType(LightType t) { type = t; }
        const GUESS::core::math::Matrix4x4& getLightSpaceMatrix() const { return m_lightSpaceMatrix; }
        const sf::Texture& getShadowMap() const { return m_shadowMap; }

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