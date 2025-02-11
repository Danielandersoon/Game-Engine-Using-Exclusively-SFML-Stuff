#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H
#include <unordered_map>
#include <functional>
#include <string>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Touch.hpp>

namespace GUESS::core {

    class InputSystem
    {
    public:
        // Key states
        enum class KeyState {
            NONE,
            PRESSED,
            HELD,
            RELEASED
        };

        InputSystem() = default;
        ~InputSystem() = default;

        void Update() {
            UpdateKeyStates();
            UpdateMouseStates();
        }

        // Keyboard methods
        bool IsKeyPressed(sf::Keyboard::Key key) const;
        bool IsKeyHeld(sf::Keyboard::Key key) const;
        bool IsKeyReleased(sf::Keyboard::Key key) const;

        // Mouse methods
        bool IsMouseButtonPressed(sf::Mouse::Button button) const;
        bool IsMouseButtonHeld(sf::Mouse::Button button) const;
        bool IsMouseButtonReleased(sf::Mouse::Button button) const;
        sf::Vector2i GetMousePosition() const;
        sf::Vector2i GetMouseDelta() const;

        // Touch methods
        bool IsTouchActive(unsigned int fingerIndex) const;
        sf::Vector2i GetTouchPosition(unsigned int fingerIndex) const;

        // Binding methods
        void BindAction(const std::string& actionName, sf::Keyboard::Key key);
        void BindAction(const std::string& actionName, sf::Mouse::Button button);
        bool IsActionTriggered(const std::string& actionName) const;

    private:
        void UpdateKeyStates();
        void UpdateMouseStates();

        std::unordered_map<sf::Keyboard::Key, KeyState> m_keyStates;
        std::unordered_map<sf::Mouse::Button, KeyState> m_mouseStates;
        std::unordered_map<std::string, sf::Keyboard::Key> m_keyBindings;
        std::unordered_map<std::string, sf::Mouse::Button> m_mouseBindings;

        sf::Vector2i m_lastMousePos;
        sf::Vector2i m_currentMousePos;
        sf::Vector2i m_mouseDelta;
    };
}

#endif // INPUT_SYSTEM_H
