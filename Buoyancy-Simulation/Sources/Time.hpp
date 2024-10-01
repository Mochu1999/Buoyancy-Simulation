#pragma once

#include <chrono>
using namespace std;
using namespace std::chrono;




struct TimeCounter {
	high_resolution_clock::time_point currentTime;
	high_resolution_clock::time_point lastTime;
	double endTime = std::numeric_limits<double>::max();
	TimeCounter() {
		currentTime = high_resolution_clock::now();
	}
	void endCounter() {
		lastTime = high_resolution_clock::now();
		endTime = duration_cast<duration<double>>(lastTime - currentTime).count();
		cout << "Elapsed time: " << endTime << "s" << endl;
	}


};





struct TimeStruct {
	std::chrono::high_resolution_clock::time_point lastFrameTime;
	std::chrono::high_resolution_clock::time_point startElapsedTime;


	float fps = 0.0f;
	float frameCount = 0.0f;
	float timeAccumulator = 0.0f;
	float deltaTime = 0.0f;

	TimeStruct() {
		lastFrameTime = std::chrono::high_resolution_clock::now();
		startElapsedTime = lastFrameTime;
	}

	void update() {
		auto currentFrameTime = std::chrono::high_resolution_clock::now();

		deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentFrameTime - lastFrameTime).count();
		lastFrameTime = currentFrameTime;

		frameCount++;
		timeAccumulator += deltaTime;

		if (timeAccumulator >= 1.0f) {
			fps = frameCount / timeAccumulator;
			frameCount = 0;
			timeAccumulator -= 1.0f;
		}
	}

	float getDeltaTime() const {
		return deltaTime;
	}

	float getFPS() const {
		return fps;
	}
};


