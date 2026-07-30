#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <vector>
#include <memory>
#include <type_traits>
#include <string>
#include "./GMath.h"
#include "./Component.h"
namespace GUESS::core {
    struct Transform {
        GUESS::core::math::Vector3f position;
        GUESS::core::math::Vector3f scale;
        GUESS::core::math::Quaternion rotation;
        GUESS::core::math::Matrix4x4 toMatrix() const {
            return GUESS::core::math::Matrix4x4::translation(position.x, position.y, position.z) *
                GUESS::core::math::Matrix4x4::rotation(rotation.x, rotation.y, rotation.z, rotation.w) *
                GUESS::core::math::Matrix4x4::scale(scale.x, scale.y, scale.z);
        }
        GUESS::core::math::Vector3f getPosition() const { return position; };
        GUESS::core::math::Quaternion getRotation() const { return rotation; };
        GUESS::core::math::Vector3f getScale() const { return scale; };
        void setPosition(const GUESS::core::math::Vector3f& pos) { position = pos; };
        void setRotation(const GUESS::core::math::Quaternion& rot) { rotation = rot; };
        void setRotation(const GUESS::core::math::Vector3f& rot) { rotation = GUESS::core::math::Quaternion::fromEuler(rot.x, rot.y, rot.z); };
        void setScale(const GUESS::core::math::Vector3f& scale) { this->scale = scale; };
    };

    static int nextGUID = 0;

	class GameObject
	{
    private:

        std::string objectName = "GameObject";
        std::vector<std::unique_ptr<Component>> componentList;
        int guid;
        int parentGuid = -1;
        std::vector<int> childGuids;
        Transform m_transform;

        bool active = true;

    public:
        GameObject() {
            guid = ++nextGUID;
        }
        GameObject(const std::string& name) : objectName(name) {
            guid = ++nextGUID;
        }
        ~GameObject() = default;

        // Component management
        template<typename T, typename... Args>
        T* addComponent(Args&&... args) {
            auto component = std::make_unique<T>(std::forward<Args>(args)...);
            T* componentPtr = component.get();
            componentList.push_back(std::move(component));
            componentPtr->setOwner(this);
            return componentPtr;
        }

        template<typename T>
        T* getComponent() const {
            for (const auto& component : componentList) {
                if (auto derived = dynamic_cast<T*>(component.get())) {
                    return derived;
                }
            }
            return nullptr;
        }

        // Hierarchy management
        void addChild(GameObject* gameObject) { childGuids.push_back(gameObject->getGUID()); };
        void removeChild(int childGUID);
        void addChild(int childGuid) { childGuids.push_back(childGuid); }
        void setParent(GameObject* gameobject) { this->parentGuid = gameobject->getGUID(); }
        void setParent(int parentGuid) { this->parentGuid = parentGuid; }
        int getParentGuid() const { return parentGuid; }
        const std::vector<int>& getChildGuids() const { return childGuids; }
        int getGUID() const { return guid; }
        void setGUID(int newGuid) { guid = newGuid; }

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

        // Called by Scene when the game object is added to a scene so that
        // components can be given a pointer to their owning scene.
        void setOwnerScene(Scene* scene);

        Transform& getTransform() { return m_transform; }
        const Transform& getTransform() const { return m_transform; }
    };
}
#endif