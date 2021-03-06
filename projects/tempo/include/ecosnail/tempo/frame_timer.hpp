#pragma once

#include <chrono>

namespace ecosnail::tempo {

class FrameTimer {
public:
    FrameTimer(int fps);
    double delta() const;
    int framesPassed();

private:
    using Clock = std::chrono::high_resolution_clock;

    long long _lastFrame;
    Clock::time_point _startTime;
    double _delta;
    Clock::duration _frameDuration;
};

} // namespace ecosnail::tempo
