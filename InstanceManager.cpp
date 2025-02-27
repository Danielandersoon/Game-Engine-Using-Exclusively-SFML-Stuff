#include "./InstanceManager.h"

namespace GUESS::core {

    InstanceManager::InstanceManager()
    {
        Initialize();
    }

    bool InstanceManager::Initialize()
    {
        Logger::log(GUESS::core::Logger::INFO, "Initializing game");
        if (!m_initialized) {
            m_initialized = true;
            m_running = true;
            Logger::log(GUESS::core::Logger::INFO, "Instance manager initialized");
            return createNewInstance(); // Creates initial instance
        }
        Logger::log(GUESS::core::Logger::ERROR, "Could not initialize the instance manager");
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
            Logger::log(GUESS::core::Logger::INFO, "Initializing engine instance");
            m_instanceCounter++;
            m_instances.push_back(std::make_unique<Engine>(m_instanceCounter));
            Logger::log(GUESS::core::Logger::INFO, "Created new engine instance");
            return true;
        }
        catch (int e) {
            Logger::log(GUESS::core::Logger::ERROR, "Could not create new engine instance");
            return false;
        }
    }

    bool InstanceManager::destroyInstance(int instanceID)
    {
        try {
            for (int x = 0; x < m_instances.size(); x++)
            {
                if (m_instances[x] -> getInstanceID() == instanceID)
                {
                    m_instances.erase(m_instances.begin() + x);
                    Logger::log(GUESS::core::Logger::INFO, "Sucessfully destroyed new engine instance ID: " + instanceID);
                    return true;
                }
            }
        }
        catch (int e) {
            Logger::log(GUESS::core::Logger::ERROR, "Could not Destroy engine ID: " + instanceID);
            return false;
        }
    }

    bool InstanceManager::pauseInstance(int instanceID)
    {
        try {
            for (int x = 0; x < m_instances.size(); x++) {
                if (m_instances[x] -> getInstanceID() == instanceID) {
                    m_instances[x] -> stop();
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
            IdVec.push_back(m_instances[x] -> getInstanceID());
        }
        return IdVec;
    }
}
