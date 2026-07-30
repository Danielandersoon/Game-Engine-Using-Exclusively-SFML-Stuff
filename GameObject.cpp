#include "./GameObject.h"

#include "./Scene.h"

namespace GUESS::core {

    void GameObject::removeChild(int childGUID) {
        auto it = std::find(childGuids.begin(), childGuids.end(), childGUID);
        if (it != childGuids.end()) {
            childGuids.erase(it);
        }
    }

    void GameObject::update() const {
        if (!active) return;

        for (auto& component : componentList) {
            component->update();
        }
    }

    void GameObject::fixedUpdate() const {
        if (!active) return;

        for (auto& component : componentList) {
            component->fixedUpdate();
        }
    }

    void GameObject::lateUpdate() const {
        if (!active) return;

        for (auto& component : componentList) {
            component->lateUpdate();
        }
    }

    void GameObject::setOwnerScene(Scene* scene) {
        for (auto& component : componentList) {
            component->setOwnerScene(scene);
            // Call init after the scene pointer is set so components can query the scene
            component->init();
        }
    }
}
