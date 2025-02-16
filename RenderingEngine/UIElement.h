#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <string>
#include <functional>

namespace GUESS::rendering::ui {
    class UIElement {
    protected:
        sf::Vector2f position;
        sf::Vector2f size;
        bool isVisible;
        std::string id;
        
    public:
        virtual void update(float deltaTime) = 0;
        virtual void render(sf::RenderTarget& target) = 0;
        virtual bool handleEvent(const sf::Event& event) = 0;
        
        void setPosition(const sf::Vector2f& pos) { position = pos; }
        void setSize(const sf::Vector2f& newSize) { size = newSize; }
        void setVisible(bool visible) { isVisible = visible; }
        const std::string& getId() const { return id; }
    };
}
#endif
