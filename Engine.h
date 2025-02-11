#ifndef ENGINE_H
#define ENGINE_H

namespace GUESS::core {

	class Engine
	{
	private:
		unsigned int m_instanceID;

	public:
		Engine(int instanceID);
		unsigned int getInstanceID();


	};
}

#endif // !ENGINE_H