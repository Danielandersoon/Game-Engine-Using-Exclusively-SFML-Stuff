#include "InputSystem.h"
namespace GUESS::core {

    void InputSystem::UpdateKeyStates() {
        for (auto& [key, state] : m_keyStates) {
            if (sf::Keyboard::isKeyPressed(key)) {
                if (state == KeyState::NONE || state == KeyState::RELEASED)
                    state = KeyState::PRESSED;
                else if (state == KeyState::PRESSED)
                    state = KeyState::HELD;
            }
            else {
                if (state == KeyState::PRESSED || state == KeyState::HELD)
                    state = KeyState::RELEASED;
                else
                    state = KeyState::NONE;
            }
        }
    }

    void InputSystem::UpdateMouseStates() {
        // Update mouse position and delta
        m_lastMousePos = m_currentMousePos;
        m_currentMousePos = sf::Mouse::getPosition();
        m_mouseDelta = m_currentMousePos - m_lastMousePos;

        // Update mouse button states
        for (auto& [button, state] : m_mouseStates) {
            if (sf::Mouse::isButtonPressed(button)) {
                if (state == KeyState::NONE || state == KeyState::RELEASED)
                    state = KeyState::PRESSED;
                else if (state == KeyState::PRESSED)
                    state = KeyState::HELD;
            }
            else {
                if (state == KeyState::PRESSED || state == KeyState::HELD)
                    state = KeyState::RELEASED;
                else
                    state = KeyState::NONE;
            }
        }
    }

    bool InputSystem::IsKeyPressed(sf::Keyboard::Key key) const {
        auto it = m_keyStates.find(key);
        return it != m_keyStates.end() && it->second == KeyState::PRESSED;
    }

    bool InputSystem::IsKeyHeld(sf::Keyboard::Key key) const {
        auto it = m_keyStates.find(key);
        return it != m_keyStates.end() && it->second == KeyState::HELD;
    }

    bool InputSystem::IsKeyReleased(sf::Keyboard::Key key) const {
        auto it = m_keyStates.find(key);
        return it != m_keyStates.end() && it->second == KeyState::RELEASED;
    }

    bool InputSystem::IsMouseButtonPressed(sf::Mouse::Button button) const {
        auto it = m_mouseStates.find(button);
        return it != m_mouseStates.end() && it->second == KeyState::PRESSED;
    }

    bool InputSystem::IsMouseButtonHeld(sf::Mouse::Button button) const {
        auto it = m_mouseStates.find(button);
        return it != m_mouseStates.end() && it->second == KeyState::HELD;
    }

    bool InputSystem::IsMouseButtonReleased(sf::Mouse::Button button) const {
        auto it = m_mouseStates.find(button);
        return it != m_mouseStates.end() && it->second == KeyState::RELEASED;
    }

    sf::Vector2i InputSystem::GetMousePosition() const {
        return m_currentMousePos;
    }

    sf::Vector2i InputSystem::GetMouseDelta() const {
        return m_mouseDelta;
    }

    bool InputSystem::IsTouchActive(unsigned int fingerIndex) const {
        return sf::Touch::isDown(fingerIndex);
    }

    sf::Vector2i InputSystem::GetTouchPosition(unsigned int fingerIndex) const {
        return sf::Touch::getPosition(fingerIndex);
    }

    void InputSystem::BindAction(const std::string& actionName, sf::Keyboard::Key key) {
        m_keyBindings[actionName] = key;
        m_keyStates[key] = KeyState::NONE;
    }

    void InputSystem::BindAction(const std::string& actionName, sf::Mouse::Button button) {
        m_mouseBindings[actionName] = button;
        m_mouseStates[button] = KeyState::NONE;
    }

    bool InputSystem::IsActionTriggered(const std::string& actionName) const {
        auto keyIt = m_keyBindings.find(actionName);
        if (keyIt != m_keyBindings.end()) {
            return IsKeyPressed(keyIt->second);
        }

        auto mouseIt = m_mouseBindings.find(actionName);
        if (mouseIt != m_mouseBindings.end()) {
            return IsMouseButtonPressed(mouseIt->second);
        }

        return false;
    }
}