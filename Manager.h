#ifndef MANAGER_H
#define MANAGER_H
#include <vector>
namespace GUESS::core {

    class Manager
    {
    public:
        // Virtual destructor for proper cleanup of derived classes
        virtual ~Manager() = default;

        // Common interface methods that all managers should implement
        virtual bool Initialize() = 0;
        virtual bool Shutdown() = 0;
        virtual bool Update() = 0;

        // Status methods
        virtual bool IsInitialized() const = 0;
        virtual bool IsRunning() const = 0;

    protected:
        bool m_initialized = false;
        bool m_running = false;
    };
}
#endif // !MANAGER_H
