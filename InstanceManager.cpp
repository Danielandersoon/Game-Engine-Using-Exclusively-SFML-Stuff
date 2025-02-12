#include "InstanceManager.h"

namespace GUESS::core {

    InstanceManager::InstanceManager()
    {
        Initialize();
    }

    bool InstanceManager::Initialize()
    {
        if (!m_initialized) {
            m_initialized = true;
            m_running = true;
            return createNewInstance(); // Creates initial instance
        }
        return true;
    }

    bool InstanceManager::Shutdown()
    {
        if (m_initialized) {
            std::vector<int> instances = GetInstanceIDs();
            for (int id : instances) {
                destroyInstance(id);
            }
            m_instances.clear();
            m_instanceCounter = 0;
            m_initialized = false;
            m_running = false;
        }
        return true;
    }

    bool InstanceManager::Update()
    {
        if (m_initialized && m_running) {
            for (auto& instance : m_instances) {
            }
            return true;
        }
        return false;
    }


    bool InstanceManager::createNewInstance()
    {
        try {
            m_instanceCounter++;
            m_instances.push_back(Engine(m_instanceCounter));
            return true;
        }
        catch (int e) {
            return false;
        }
    }

    bool InstanceManager::destroyInstance(int instanceID)
    {
        try {
            for (int x = 0; x < m_instances.size(); x++)
            {
                if (m_instances[x].getInstanceID() == instanceID)
                {
                    m_instances.erase(m_instances.begin() + x);
                    return true;
                }
            }
        }
        catch (int e) {
            return false;
        }
    }

    bool InstanceManager::pauseInstance(int instanceID)
    {
        try {
            for (int x = 0; x < m_instances.size(); x++) {
                if (m_instances[x].getInstanceID() == instanceID) {
                    m_instances[x].stop();
                    return true;
                }
            }
        }
        catch (int e) {
            return false;
        }
    }

    unsigned int InstanceManager::getInstanceID(Engine* instance_ptr)
    {
        return instance_ptr->getInstanceID();
    }

    std::vector<int> InstanceManager::GetInstanceIDs()
    {
        std::vector<int> IdVec;
        for (int x = 0; x < m_instances.size(); x++)
        {
            IdVec.push_back(m_instances[x].getInstanceID());
        }
        return IdVec;
    }
}
