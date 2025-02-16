#include "UISystem.h"

namespace GUESS::rendering::ui {
    void UISystem::initialize(sf::RenderTarget* renderTarget) {
        target = renderTarget;
    }

    void UISystem::update(float deltaTime) {
        for (auto& element : elements) {
            element->update(deltaTime);
        }
    }

    void UISystem::render() {
        for (auto& element : elements) {
            element->render(*target);
        }
    }

    void UISystem::addElement(std::unique_ptr<UIElement> element) {
        elements.push_back(std::move(element));
    }

    void UISystem::removeElement(const std::string& id) {
        elements.erase(
            std::remove_if(elements.begin(), elements.end(),
                [&id](const auto& element) { return element->getId() == id; }),
            elements.end()
        );
    }
}
