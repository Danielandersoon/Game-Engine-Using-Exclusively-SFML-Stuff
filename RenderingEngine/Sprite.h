#ifndef  SPRITE_H
#define SPRITE_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace GUESS::rendering::twod {
    class Sprite {
    private:
        sf::Texture texture;
        sf::Vector2f position;
        sf::Vector2f scale;
        float rotation;

    public:
        void setTexture(const sf::Texture& texture) {this->texture = texture;};
        void setPosition(const sf::Vector2f& position) {this->position = position;};
        void setScale(const sf::Vector2f& scale) {this->scale = scale;};
        void setRotation(float rotation) {this->rotation = rotation;};
        const sf::Texture& getTexture() const {return texture;};
        const sf::Vector2f& getPosition() const { return position; };
        const sf::Vector2f& getScale() const { return scale; };
        float getRotation() const { return rotation; };

        sf::Sprite toSFMLSprite() const {
            sf::Sprite sprite;
            sprite.setTexture(texture);
            sprite.setPosition(position);
            sprite.setScale(scale);
            sprite.setRotation(rotation);
            return sprite;
        }

    };
}

#endif