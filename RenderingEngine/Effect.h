#ifndef EFFECT_H
#define EFFECT_H
#include <vector>
#include <memory>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>

namespace GUESS::rendering {
    
    class Effect {
    protected:
        sf::Shader shader;
        bool enabled = true;
        
    public:
        virtual ~Effect() = default;
        
        virtual void apply(const sf::RenderTexture& input, sf::RenderTexture& output) = 0;
        
        void setEnabled(bool state) { enabled = state; }
        bool isEnabled() const { return enabled; }
        sf::Shader* getShader() { return &shader; }
    };
}
#endif
