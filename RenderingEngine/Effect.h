#ifndef EFFECT_H
#define EFFECT_H

#include <SFML/Graphics/RenderTexture.hpp>
#include "./Shader.h"

namespace GUESS::rendering {
    class Effect {
    protected:
        GUESS::rendering::Shader shader;
        bool enabled = true;

    public:
        virtual ~Effect() = default;
        virtual void apply(const sf::RenderTexture& input, sf::RenderTexture& output) = 0;
        void setEnabled(bool state) { enabled = state; }
        bool isEnabled() const { return enabled; }
    };

    // Example concrete effects
    class BloomEffect : public Effect {
    public:
        BloomEffect() {
            shader.loadFromFile("shaders/bloom.vert", "shaders/bloom.frag");
        }
        void apply(const sf::RenderTexture& input, sf::RenderTexture& output) override;
        void setThreshold(float value) { shader.setUniform("threshold", value); }
        void setIntensity(float value) { shader.setUniform("intensity", value); }
    };

    class BlurEffect : public Effect {
    public:
        BlurEffect() {
            shader.loadFromFile("shaders/blur.vert", "shaders/blur.frag");
        }
        void apply(const sf::RenderTexture& input, sf::RenderTexture& output) override;
        void setRadius(float value) { shader.setUniform("radius", value); }
    };
}
#endif
