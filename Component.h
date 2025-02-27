#ifndef COMPONENT_H 
#define COMPONENT_H
namespace GUESS::core {

	class GameObject;
	//
	// This is a base class.
	// There should not be any implementation.
	//
	class Component
	{
    protected:
        GameObject* owner = nullptr;
        std::string name;

    public:
        virtual ~Component() = default;

        virtual void init() {}
        virtual void update() const {}
        virtual void fixedUpdate() const {}
        virtual void lateUpdate() const {}

        // except this implementation... oops
        void setOwner(GameObject* newOwner) { owner = newOwner; }
        GameObject* getOwner() const { return owner; }
        void setName(std::string name) { this->name = name; }
        std::string getName() { return name; }
    };
}

#endif