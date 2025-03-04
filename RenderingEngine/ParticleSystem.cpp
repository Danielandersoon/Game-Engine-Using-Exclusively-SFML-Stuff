#include "./ParticleSystem.h"

namespace GUESS::rendering {

    ParticleSystem::ParticleSystem(unsigned int count) :
        maxParticles(count),
        activeParticles(0),
        emissionRate(0),
        emissionAccumulator(0) {

        // Initialize particle pool
        particlePool.resize(maxParticles);
        particles.reserve(maxParticles);

        // Setup vertex buffer for instancing
        vertices.setPrimitiveType(sf::Quads);
        vertices.resize(4); // Base quad for instancing

        // Setup base quad vertices
        float halfSize = 0.5f;
        vertices[0].position = sf::Vector2f(-halfSize, -halfSize);
        vertices[1].position = sf::Vector2f(halfSize, -halfSize);
        vertices[2].position = sf::Vector2f(halfSize, halfSize);
        vertices[3].position = sf::Vector2f(-halfSize, halfSize);

        // Setup UV coordinates
        vertices[0].texCoords = sf::Vector2f(0, 0);
        vertices[1].texCoords = sf::Vector2f(1, 0);
        vertices[2].texCoords = sf::Vector2f(1, 1);
        vertices[3].texCoords = sf::Vector2f(0, 1);
    }

    void ParticleSystem::update(float deltaTime) {
        // Update emission
        emissionAccumulator += deltaTime * emissionRate;

        // Update physics and lifetime
        updatePhysics(deltaTime);

        // Update instance transforms
        instanceTransforms.clear();
        for (size_t i = 0; i < activeParticles; ++i) {
            Particle& p = particlePool[i];

            // Calculate alpha based on lifetime
            float alpha = p.lifetime / p.initialLifetime;
            sf::Uint8 colorAlpha = static_cast<sf::Uint8>(alpha * 255);

            // Create transform matrix
            sf::Transform transform;
            transform.translate(p.position.x, p.position.y);
            transform.scale(p.size, p.size);

            instanceTransforms.push_back(transform);
        }
    }

    void ParticleSystem::updatePhysics(float deltaTime) {
        for (size_t i = 0; i < activeParticles;) {
            Particle& p = particlePool[i];

            p.lifetime -= deltaTime;
            if (p.lifetime <= 0) {
                recyclePArticle(i);
                continue;
            }

            // Update physics
            p.velocity = p.velocity + (p.acceleration * deltaTime);
            p.position = p.position + (p.velocity * deltaTime);

            ++i;
        }
    }

    void ParticleSystem::render(sf::RenderTarget& target) {
        if (activeParticles == 0) return;

        sf::RenderStates states;
        states.texture = &particleTexture;
        states.blendMode = sf::BlendAdd;

        // Render all particles using instancing
        for (const auto& transform : instanceTransforms) {
            states.transform = transform;
            target.draw(vertices, states);
        }
    }

    Particle* ParticleSystem::getNextFreeParticle() {
        if (activeParticles >= maxParticles) return nullptr;
        return &particlePool[activeParticles++];
    }

    void ParticleSystem::recyclePArticle(size_t index) {
        if (index >= activeParticles) return;

        // Move last active particle to this slot
        if (index < activeParticles - 1) {
            particlePool[index] = particlePool[activeParticles - 1];
        }
        --activeParticles;
    }
}
