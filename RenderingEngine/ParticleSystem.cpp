#include "ParticleSystem.h"

namespace GUESS::rendering {
    
    ParticleSystem::ParticleSystem(unsigned int count) : maxParticles(count) {
        particles.reserve(maxParticles);
        vertices.setPrimitiveType(sf::Points);
    }

    void ParticleSystem::update(float deltaTime) {
        // Update existing particles
        for (auto it = particles.begin(); it != particles.end();) {
            it->lifetime -= deltaTime;
            if (it->lifetime <= 0) {
                it = particles.erase(it);
            } else {
                it->position = it->position + it->velocity * deltaTime;
                it->color.a = static_cast<sf::Uint8>((it->lifetime / 2.0f) * 255);
                ++it;
            }
        }

        // Update vertex array
        vertices.clear();
        for (const auto& particle : particles) {
            sf::Vertex vertex;
            vertex.position = sf::Vector2f(particle.position.x, particle.position.y);
            vertex.color = particle.color;
            vertex.texCoords = sf::Vector2f(0, 0);
            vertices.append(vertex);
        }
    }

    void ParticleSystem::emit(const GUESS::core::math::Vector3f& position,
                            const GUESS::core::math::Vector3f& direction,
                            float spread,
                            float speed,
                            float lifetime,
                            float size,
                            const sf::Color& color) {
        if (particles.size() >= maxParticles) return;

        GUESS::core::math::Random random;
        
        Particle particle;
        particle.position = position;
        
        // Calculate random direction within spread cone
        float angle = random.nextFloat(-spread, spread);
        GUESS::core::math::Quaternion rotation = 
            GUESS::core::math::Quaternion::fromEuler(0, 0, angle);
        particle.velocity = rotation * direction * speed;
        
        particle.color = color;
        particle.lifetime = lifetime;
        particle.size = size;
        
        particles.push_back(particle);
    }

    void ParticleSystem::render(sf::RenderTarget& target) {
        sf::RenderStates states;
        states.texture = &particleTexture;
        target.draw(vertices, states);
    }

    void ParticleSystem::setTexture(const std::string& texturePath) {
        particleTexture.loadFromFile(texturePath);
    }
}
