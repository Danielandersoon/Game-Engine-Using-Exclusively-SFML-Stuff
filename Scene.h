#ifndef SCENE_H
#define SCENE_H
#include <string>
#include "GameObject.h"
namespace GUESS::core {

	class Scene
	{
	public:
		Scene(std::string sceneName);
		unsigned int GetSceneID() { return ID; };
		std::string GetSceneName() { return m_sceneName; };
		bool LoadScene();
		bool CloseScene();
	private:
		std::vector <GameObject> GameObjects;
		unsigned int ID;
		std::string m_sceneName;
		bool m_active;
	};
}

#endif // !SCENE_H