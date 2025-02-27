#ifndef SPRITE_RENDERER_COMPONENT_H
#define SPRITE_RENDERER_COMPONENT_H

#include "./Component.h"
#include "./RenderingEngine/Sprite.h"
#include "./RenderingEngine/Material.h"
#include <memory>
#include <string>

namespace GUESS::core {
    class SpriteRendererComponent : public Component {
    public:
        // Public attributes
        std::string spritePath;
        GUESS::rendering::twod::Sprite sprite;
        std::shared_ptr<GUESS::rendering::Material> material;
        sf::Color color = sf::Color::White;
        bool flipX = false;
        bool flipY = false;

        void init() override {
            setName("sprite render component");
            if (!spritePath.empty()) {
                loadSprite(spritePath);
            }
        }

        void loadSprite(const std::string& path) {
            sf::Texture texture;
            if (texture.loadFromFile(path)) {
                sprite.setTexture(texture);
                spritePath = path;
            }
        }

        void setMaterial(std::shared_ptr<GUESS::rendering::Material> newMaterial) {
            material = newMaterial;
        }
    };
}

#endif
