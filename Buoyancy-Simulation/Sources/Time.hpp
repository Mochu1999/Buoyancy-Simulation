#pragma once

#include <chrono>
using namespace std;
using namespace std::chrono;




struct TimeCounter {
    high_resolution_clock::time_point currentTime;
    high_resolution_clock::time_point lastTime;

    TimeCounter() {
        currentTime = high_resolution_clock::now();
    }
    void endCounter() {
        lastTime = high_resolution_clock::now();
        cout << "Elapsed time: " << duration_cast<duration<float>>(lastTime - currentTime).count() << endl;
    }


};





struct TimeData {
    std::chrono::high_resolution_clock::time_point lastFrameTime;
    std::chrono::high_resolution_clock::time_point startElapsedTime;


    float fps = 0.0f;
    float frameCount = 0.0f;
    float timeAccumulator = 0.0f;
    float deltaTime = 0.0f;
    TimeData() {
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


