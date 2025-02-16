#ifndef UI_SYSTEM_H
#define UI_SYSTEM_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <memory>
#include <vector>
#include "UIElement.h"

namespace GUESS::rendering::ui {
    class UISystem {
    private:
        std::vector<std::unique_ptr<UIElement>> elements;
        sf::RenderTarget* target;

    public:
        void initialize(sf::RenderTarget* renderTarget);
        void update(float deltaTime);
        void render();
        void addElement(std::unique_ptr<UIElement> element);
        void removeElement(const std::string& id);
    };
}
#endif
