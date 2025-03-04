#include "./Effect.h"
#include <SFML/Graphics/Sprite.hpp>

namespace GUESS::rendering {
    void BloomEffect::apply(const sf::RenderTexture& input, sf::RenderTexture& output) {
        if (!enabled) {
            output.clear();
            output.draw(sf::Sprite(input.getTexture()));
            output.display();
            return;
        }

        shader.bind();
        shader.setUniform("inputTexture", input.getTexture());

        sf::Sprite sprite(input.getTexture());
        output.clear();
        output.draw(sprite, shader.getNativeShader());  // Get native shader before binding
        output.display();
    }

    void BlurEffect::apply(const sf::RenderTexture& input, sf::RenderTexture& output) {
        if (!enabled) {
            output.clear();
            output.draw(sf::Sprite(input.getTexture()));
            output.display();
            return;
        }

        shader.bind();
        shader.setUniform("inputTexture", input.getTexture());

        sf::Sprite sprite(input.getTexture());
        output.clear();
        output.draw(sprite, shader.getNativeShader());  // Get native shader before binding
        output.display();
    }

}
