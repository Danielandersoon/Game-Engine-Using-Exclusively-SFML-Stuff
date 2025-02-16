#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H
#include <vector>
#include <SFML/Graphics/RenderTexture.hpp>
#include "Effect.h"

namespace GUESS::rendering
{
    class PostProcessor {
    public:
        void initialize(int width, int height);
        void process();
    private:
        std::vector<std::unique_ptr<Effect>> effects;
        sf::RenderTexture mainBuffer;
        sf::RenderTexture pingPongBuffer;
    };
}
#endif