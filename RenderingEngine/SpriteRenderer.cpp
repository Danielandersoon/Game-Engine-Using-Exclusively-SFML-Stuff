#include "./SpriteRenderer.h"

namespace GUESS::rendering::twod {
    void SpriteRenderer::draw(const Sprite& sprite, Material& material) {
        material.bind();

        sf::RenderStates states;
        states.shader = material.getShader().getNativeShader(); 
        states.texture = &sprite.getTexture();

        sf::Sprite sfmlSprite;
        sfmlSprite.setTexture(sprite.getTexture());
        sfmlSprite.setPosition(sprite.getPosition());
        sfmlSprite.setScale(sprite.getScale());
        sfmlSprite.setRotation(sprite.getRotation());

        m_renderTarget->draw(sfmlSprite, states);
    }

}

