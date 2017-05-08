#pragma once
#include <GLFW\glfw3.h>

#define Time TimeManager::Instance()

class TimeManager {
public:
	inline static TimeManager &Instance(void) {
		static TimeManager timeManager;
		return timeManager;
	}
	inline void Update() {
		deltaTime = glfwGetTime() - lastTime;
		lastTime = deltaTime;
	}
	inline float DeltaTime() {
		return deltaTime;
	}
	inline float GetTime(){
		return glfwGetTime();
	}

private:
	TimeManager() = default;
	TimeManager(const TimeManager &rhs) = delete;
	TimeManager &operator=(const TimeManager &rhs) = delete;

	float deltaTime{ 0.f };
	float lastTime{ 0.f };
	float time{ 0.f };
};