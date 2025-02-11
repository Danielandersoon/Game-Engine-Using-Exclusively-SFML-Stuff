#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <vector>
#include "Component.h"
namespace GUESS::core {

	class GameObject
	{
	private:
		std::string objectName = "GameObject";

		std::vector<Component> componentList;
		GameObject* parentObject = 0;		   // staarts as null assuming there is no parent. this can be set on creation
		std::vector<GameObject*> childObjects; /* this only stores the pointer to the direct child.
												  any grand-children are stored in this attribute in the direct child */
	};
}
#endif GAME_OBJECCT_H