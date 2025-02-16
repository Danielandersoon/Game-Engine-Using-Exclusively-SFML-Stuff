#include "PostProcessor.h"
#include <SFML/Graphics/Sprite.hpp>

namespace GUESS::rendering {

    void PostProcessor::initialize(int width, int height) {
        mainBuffer.create(width, height);
        pingPongBuffer.create(width, height);
    }

    void PostProcessor::process() {
        for (const auto& effect : effects) {
            mainBuffer.display();
            pingPongBuffer.clear();

            // Create sprite from main buffer's texture
            sf::Sprite sprite(mainBuffer.getTexture());

            // Draw to ping pong buffer
            pingPongBuffer.draw(sprite);
            pingPongBuffer.display();

            // Copy ping pong buffer back to main buffer
            mainBuffer.clear();
            sprite.setTexture(pingPongBuffer.getTexture());
            mainBuffer.draw(sprite);
        }
    }
}
