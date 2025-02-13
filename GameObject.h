#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <vector>
#include <memory>
#include <type_traits>
#include <string>
#include "Component.h"
namespace GUESS::core {

	class GameObject
	{
    private:
        std::string objectName = "GameObject";
        std::vector<std::unique_ptr<Component>> componentList;
        GameObject* parentObject = 0;		   // starts as null assuming there is no parent. this can be set on creation
        std::vector<std::unique_ptr<GameObject>> childObjects; /* this only stores the pointer to the direct child.
                                                                  any grand-children are stored in this attribute in the direct child */

        bool active = true;

    public:
        GameObject() = default;
        explicit GameObject(const std::string& name);
        ~GameObject() = default;

        // Component management
        template<typename T>
        T* addComponent() {
            static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
            auto component = std::make_unique<T>();
            component->setOwner(this);
            T* rawPtr = component.get();
            componentList.push_back(std::move(component));
            return rawPtr;
        }

        template<typename T>
        T* getComponent() {
            for (auto& component : componentList) {
                if (T* cast = dynamic_cast<T*>(component.get()))
                    return cast;
            }
            return nullptr;
        }

        // Hierarchy management
        void addChild(std::unique_ptr<GameObject> child);
        void removeChild(GameObject* child);
        GameObject* getParent() const { return parentObject; }

        // Object state
        void setActive(bool state) { active = state; }
        bool isActive() const { return active; }

        // Name management
        void setName(const std::string& name) { objectName = name; }
        const std::string& getName() const { return objectName; }

        // Update methods
        void update() const;
        void fixedUpdate() const;
        void lateUpdate() const;
    };
}
#endif GAME_OBJECCT_H