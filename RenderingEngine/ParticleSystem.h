#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "../GMath.h"

namespace GUESS::rendering {
    struct Particle {
        GUESS::core::math::Vector3f position;
        GUESS::core::math::Vector3f velocity;
        GUESS::core::math::Vector3f acceleration;
        sf::Color color;
        float lifetime;
        float initialLifetime;
        float size;
    };

    class ParticleSystem {
    private:
        std::vector<Particle> particles;
        sf::VertexArray vertices;
        sf::Texture particleTexture;
        unsigned int maxParticles;

        std::vector<Particle> particlePool;
        size_t activeParticles;

        sf::VertexBuffer instanceBuffer;
        std::vector<sf::Transform> instanceTransforms;

        float emissionRate;
        float emissionAccumulator;
        
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
        void setEmissionRate(float particlesPerSecond);
        void updatePhysics(float deltaTime);
        void recyclePArticle(size_t index);
        Particle* getNextFreeParticle();

    };
}
#endif
