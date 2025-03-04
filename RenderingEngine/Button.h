#ifndef UI_BUTTON_H
#define UI_BUTTON_H
#include "./UIElement.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Color.hpp>

namespace GUESS::rendering::ui {
    class Button : public UIElement {
    private:
        sf::RectangleShape background;
        sf::Text text;
        std::function<void()> onClick;
        sf::Color normalColor;
        sf::Color hoverColor;
        sf::Color pressedColor;
        bool isHovered;
        bool isPressed;
        
    public:
        Button(const std::string& buttonText, const sf::Font& font);
        void setOnClick(std::function<void()> callback);
        void setColors(sf::Color normal, sf::Color hover, sf::Color pressed);
        void update(float deltaTime) override;
        void render(sf::RenderTarget& target) override;
        bool handleEvent(const sf::Event& event) override;
    };
}
#endif
