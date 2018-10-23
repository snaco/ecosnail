#include <ecosnail/dimentia.hpp>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <string>

using ecosnail::dimentia::Dimented;

template <int> struct Dots {};
template <int> struct Seconds {};

using Position = Dimented<float, Dots<1>>;
using Speed = Dimented<float, Dots<1>, Seconds<-1>>;
using Acceleration = Dimented<float, Dots<1>, Seconds<-2>>;
using Time = Dimented<float, Seconds<1>>;

struct RunningSquare {
    static constexpr auto MIDDLE = Position{40};
    static constexpr auto ACCELERATION = Acceleration{50};

    void update(Time delta)
    {
        acceleration = -ACCELERATION + (position < MIDDLE) * 2 * ACCELERATION;
        speed += acceleration * delta;
        position += speed * delta;
        position = std::clamp(position, Position{0}, 2 * MIDDLE);
    }

    void draw() const
    {
        auto integerPosition = std::lround(position.value());
        std::cout << '\r' << std::string(integerPosition, ' ') << "\u2588" <<
            std::string(2 * MIDDLE.value() - integerPosition, ' ');
        std::cout.flush();
    }

    Position position = Position{0};
    Speed speed = Speed{0};
    Acceleration acceleration;
};

using Clock = std::chrono::high_resolution_clock;
using DoubleSeconds = std::chrono::duration<double>;

constexpr auto FPS = 30;
constexpr auto RAW_FRAME_DURATION = 1.0 / FPS;
constexpr auto FRAME_DURATION = std::chrono::duration_cast<Clock::duration>(
    std::chrono::duration<double>(RAW_FRAME_DURATION));

int main()
{
    RunningSquare square;

    auto previousTime = Clock::now();
    while (true) {
        auto currentTime = Clock::now();
        if (currentTime >= previousTime + FRAME_DURATION) {
            previousTime = currentTime;
            square.update(Time{RAW_FRAME_DURATION});
            square.draw();
        }
    }
}
