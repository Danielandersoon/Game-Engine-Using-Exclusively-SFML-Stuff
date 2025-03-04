#include "./Button.h"

namespace GUESS::rendering::ui {
    Button::Button(const std::string& buttonText, const sf::Font& font) {
        text.setFont(font);
        text.setString(buttonText);
        text.setFillColor(sf::Color::Black);
        text.setCharacterSize(24);

        // Default colors
        normalColor = sf::Color::White;
        hoverColor = sf::Color(230, 230, 230);
        pressedColor = sf::Color(200, 200, 200);

        background.setFillColor(normalColor);
        background.setOutlineColor(sf::Color::Black);
        background.setOutlineThickness(2.0f);

        isHovered = false;
        isPressed = false;
    }

    void Button::setColors(sf::Color normal, sf::Color hover,  sf::Color pressed) {
        normalColor = normal;
        hoverColor = hover;
        pressedColor = pressed;
        background.setFillColor(normalColor);
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

        // Update visual state
        if (isPressed) {
            background.setFillColor(pressedColor);
        }
        else if (isHovered) {
            background.setFillColor(hoverColor);
        }
        else {
            background.setFillColor(normalColor);
        }
    }

    void Button::render(sf::RenderTarget& target) {
        if (!isVisible) return;

        target.draw(background);
        target.draw(text);
    }

    bool Button::handleEvent(const sf::Event& event) {
        if (!isVisible) return false;

        sf::FloatRect bounds(position, size);
        sf::Vector2f mousePos;

        switch (event.type) {
        case sf::Event::MouseMoved:
            mousePos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
            isHovered = bounds.contains(mousePos);
            return isHovered;

        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left) {
                mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                if (bounds.contains(mousePos)) {
                    isPressed = true;
                    return true;
                }
            }
            break;

        case sf::Event::MouseButtonReleased:
            if (event.mouseButton.button == sf::Mouse::Left) {
                mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                if (isPressed && bounds.contains(mousePos)) {
                    if (onClick) onClick();
                }
                isPressed = false;
                return true;
            }
            break;
        }
        return false;
    }

    void Button::setOnClick(std::function<void()> callback) {
        onClick = callback;
    }
}
