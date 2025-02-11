#ifndef TIMING_SYSTEM_H
#define TIMING_SYSTEM_H
#include <chrono>
namespace GUESS::core {
	class TimingSystem
	{
	private:
		float dt;
		float timeScale = 1.0f;

		std::chrono::high_resolution_clock::time_point prevTime = std::chrono::high_resolution_clock::now();

	public:
		void TimingUpdate();
		int DeltaTime() { return dt; }
	};
}

#endif TIMING_SYSTEM_H