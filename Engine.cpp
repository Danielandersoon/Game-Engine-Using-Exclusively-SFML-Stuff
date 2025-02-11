#include "Engine.h"
namespace GUESS::core {

    Engine::Engine(int instanceID)
    {
    }

    unsigned int Engine::getInstanceID()
    {
        return this->m_instanceID;
    }
}
