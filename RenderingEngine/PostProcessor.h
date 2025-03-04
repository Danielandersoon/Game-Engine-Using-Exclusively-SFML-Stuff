#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H
#include <vector>
#include <SFML/Graphics/RenderTexture.hpp>
#include "./Effect.h"

namespace GUESS::rendering
{
    class PostProcessor {
    public:
        void initialize(int width, int height);
        void process();
    private:
        static inline void swapBuffers(sf::RenderTexture& a, sf::RenderTexture& b) {
            sf::Vector2u size = a.getSize();
            sf::Texture tempTexture = a.getTexture();

            a.create(b.getSize().x, b.getSize().y);
            a.draw(sf::Sprite(b.getTexture()));
            a.display();

            b.create(size.x, size.y);
            b.draw(sf::Sprite(tempTexture));
            b.display();
        }
        std::vector<std::unique_ptr<Effect>> effects;
        sf::RenderTexture mainBuffer;
        sf::RenderTexture pingPongBuffer;
    };
}
#endif