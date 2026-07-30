#ifndef COMPONENT_H 
#define COMPONENT_H
#include <memory>
#include <string>
namespace GUESS::core {

    class Scene;
    class GameObject;

	//
	// This is a base class.
	// There should not be any implementation.
	//
	class Component
	{

    protected:
        // Store raw pointer to the owner scene. Scene lifetime is managed elsewhere
        // and components should not extend scene lifetime, so a raw pointer is
        // appropriate and avoids shared ownership cycles.
        GUESS::core::Scene* m_ownerScene = nullptr;
        int owner = -1;
        std::string name;

    public:
        virtual ~Component() = default;

        virtual void init() {}
        virtual void update() const {}
        virtual void fixedUpdate() const {}
        virtual void lateUpdate() const {}

        // except this implementation... oops
        void setOwnerScene(Scene* scene) { m_ownerScene = scene; }
        void setOwner(int newOwner) { owner = newOwner; }
        void setOwner(GameObject* newOwner);
        int getOwner() const { return owner; }
        void setName(std::string name) { this->name = name; }
        std::string getName() { return name; }
    };
}

#endif