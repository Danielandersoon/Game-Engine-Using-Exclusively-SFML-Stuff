#ifndef INSTANCE_MANAGER_H
#define INSTANCE_MANAGER_H
#include <vector>
#include "Manager.h"
#include "Engine.h"
namespace GUESS::core {

    class InstanceManager : Manager
    {
    private:
        std::vector<std::unique_ptr<Engine>> m_instances;
        int m_instanceCounter = 0;

    public:
        InstanceManager();
        virtual ~InstanceManager() override = default;

        virtual bool Initialize() override;
        virtual bool Shutdown() override;
        virtual bool Update() override;
        virtual bool IsInitialized() const override { return m_initialized; }
        virtual bool IsRunning() const override { return m_running; }

        bool createNewInstance();
        bool destroyInstance(int instanceID);
        bool pauseInstance(int instanceID);
        unsigned int getInstanceID(Engine* instance_ptr);
        std::vector<int> GetInstanceIDs();

    };
}

#endif // !INSTANCE_MANAGER_H