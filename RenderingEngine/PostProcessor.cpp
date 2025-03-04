#include "./PostProcessor.h"
#include <algorithm>
#include <SFML/Graphics/Sprite.hpp>

namespace GUESS::rendering {

    void PostProcessor::initialize(int width, int height) {
        mainBuffer.create(width, height);
        pingPongBuffer.create(width, height);
    }

    void PostProcessor::process() {
        for (const auto& effect : effects) {
            if (effect->isEnabled()) {
                effect->apply(mainBuffer, pingPongBuffer);
                swapBuffers(mainBuffer, pingPongBuffer);
            }
        }
    }
}
