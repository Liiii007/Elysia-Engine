#include <stdafx.h>
export module Timer;

namespace Timer {
	using tp = std::chrono::steady_clock::time_point;

	//---Normal Record event
	std::unordered_map<std::string, tp> eventList{};
	//Record the time point at event call(multi-call will record last time point)
	export void startRecord(const std::string& event) {
		eventList[event] = std::chrono::steady_clock::now();
	}
	//Get last time point and calu time during
	export float stopRecord(const std::string& event) {
		if (eventList.contains(event)) {
			tp currentTime = std::chrono::steady_clock::now();
			return std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - eventList[event]).count();
		}
		else {
			return -1;
		}

	}

	//---Timer for renderer
	tp startTimePoint{ std::chrono::steady_clock::now() };
	tp lastTickTimePoint;
	float pauseTime{ 0 };
	float deltaTime{ 0.01667 };
	bool isPaused{ false };

	export void reset() {
		startTimePoint = std::chrono::steady_clock::now();
	}
	//Warning::Only call in main loop
	export void Tick() {
		tp currentTime = std::chrono::steady_clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - lastTickTimePoint).count();
		lastTickTimePoint = currentTime;
	}
	export float GetDeltaTime() {
		return deltaTime;
	}
	export float totalTime() {
		tp currentTime = std::chrono::steady_clock::now();
		return std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - startTimePoint).count() - pauseTime;
	}

	//One pause, One resume
	export void pause() {
		if (!isPaused) {
			startRecord("___PAUSE___@");
			isPaused = true;
		}
	}
	export void resume() {
		if (isPaused) {
			pauseTime += stopRecord("___PAUSE___@");
			isPaused = false;
		}
	}
}