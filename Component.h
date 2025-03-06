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
        std::shared_ptr<GUESS::core::Scene> m_ownerScene;
        int owner = -1;
        std::string name;

    public:
        virtual ~Component() = default;

        virtual void init() {}
        virtual void update() const {}
        virtual void fixedUpdate() const {}
        virtual void lateUpdate() const {}

        // except this implementation... oops
        void setOwnerScene(std::shared_ptr<Scene> scene) { m_ownerScene = scene; }
        void setOwner(int newOwner) { owner = newOwner; }
        void setOwner(GameObject* newOwner);
        int getOwner() const { return owner; }
        void setName(std::string name) { this->name = name; }
        std::string getName() { return name; }
    };
}

#endif