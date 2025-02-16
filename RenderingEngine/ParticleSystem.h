#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "../GMath.h"

namespace GUESS::rendering {
    struct Particle {
        GUESS::core::math::Vector3f position;
        GUESS::core::math::Vector3f velocity;
        sf::Color color;
        float lifetime;
        float size;
    };

    class ParticleSystem {
    private:
        std::vector<Particle> particles;
        sf::VertexArray vertices;
        sf::Texture particleTexture;
        unsigned int maxParticles;
        
    public:
        ParticleSystem(unsigned int count = 1000);
        void update(float deltaTime);
        void emit(const GUESS::core::math::Vector3f& position, 
                 const GUESS::core::math::Vector3f& direction,
                 float spread,
                 float speed,
                 float lifetime,
                 float size,
                 const sf::Color& color);
        void render(sf::RenderTarget& target);
        void setTexture(const std::string& texturePath);
    };
}
#endif
