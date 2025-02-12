#include "GameObject.h"

namespace GUESS::core {

    GameObject::GameObject(const std::string& name) : objectName(name) {}

    void GameObject::addChild(std::unique_ptr<GameObject> child) {
        child->parentObject = this;
        childObjects.push_back(std::move(child));
    }

    void GameObject::removeChild(GameObject* child) {
        auto it = std::find_if(childObjects.begin(), childObjects.end(),
            [child](const std::unique_ptr<GameObject>& obj) {
            return obj.get() == child;
        });
        if (it != childObjects.end()) {
            (*it)->parentObject = nullptr;
            childObjects.erase(it);
        }
    }

    void GameObject::update() {
        if (!active) return;

        for (auto& component : componentList) {
            component->update();
        }

        for (auto& child : childObjects) {
            child->update();
        }
    }

    void GameObject::fixedUpdate() {
        if (!active) return;

        for (auto& component : componentList) {
            component->fixedUpdate();
        }

        for (auto& child : childObjects) {
            child->fixedUpdate();
        }
    }

    void GameObject::lateUpdate() {
        if (!active) return;

        for (auto& component : componentList) {
            component->lateUpdate();
        }

        for (auto& child : childObjects) {
            child->lateUpdate();
        }
    }
}
