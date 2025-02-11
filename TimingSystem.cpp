#include "TimingSystem.h"
namespace GUESS::core {

    void TimingSystem::TimingUpdate()
    {
        std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
        dt = std::chrono::duration<float>(currentTime - prevTime).count() / timeScale;
        prevTime = currentTime;
    }
}