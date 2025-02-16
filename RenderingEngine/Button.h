#ifndef UI_BUTTON_H
#define UI_BUTTON_H
#include "UIElement.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

namespace GUESS::rendering::ui {
    class Button : public UIElement {
    private:
        sf::RectangleShape background;
        sf::Text text;
        std::function<void()> onClick;
        
    public:
        Button(const std::string& buttonText, const sf::Font& font);
        void setOnClick(std::function<void()>* callback);
        void update(float deltaTime) override;
        void render(sf::RenderTarget& target) override;
        bool handleEvent(const sf::Event& event) override;
    };
}
#endif
