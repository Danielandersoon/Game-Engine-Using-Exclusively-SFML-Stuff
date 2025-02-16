#include "Button.h"

namespace GUESS::rendering::ui {
    Button::Button(const std::string& buttonText, const sf::Font& font) {
        text.setFont(font);
        text.setString(buttonText);
        text.setFillColor(sf::Color::Black);
        
        background.setFillColor(sf::Color::White);
        background.setOutlineColor(sf::Color::Black);
        background.setOutlineThickness(2.0f);
    }

    void Button::setOnClick(std::function<void()>* callback) {
        onClick = *callback;
    }

    void Button::update(float deltaTime) {
        // Update text position relative to background
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(
            position.x + (size.x - textBounds.width) / 2.0f,
            position.y + (size.y - textBounds.height) / 2.0f
        );
        
        background.setPosition(position);
        background.setSize(size);
    }

    void Button::render(sf::RenderTarget& target) {
        if (!isVisible) return;
        
        target.draw(background);
        target.draw(text);
    }

    bool Button::handleEvent(const sf::Event& event) {
        if (!isVisible) return false;

        if (event.type == sf::Event::MouseButtonPressed) {
            sf::FloatRect bounds(position, size);
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            
            if (bounds.contains(mousePos)) {
                if (onClick) onClick();
                return true;
            }
        }
        return false;
    }
}
