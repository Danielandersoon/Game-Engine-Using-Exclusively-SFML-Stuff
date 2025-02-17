#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "Sprite.h"
#include "Material.h"

#include "Sprite.h"
#include "Material.h"
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUESS::rendering::twod {
    class SpriteRenderer {
    private:
        sf::RenderTarget* m_renderTarget;

    public:
        SpriteRenderer(sf::RenderTarget* target) : m_renderTarget(target) {}
        void draw(const Sprite& sprite, Material& material);
    };
}


#endif