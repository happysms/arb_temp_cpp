#pragma once
#include <cmath>
#include <chrono>

inline double floor_by_tick(double value, double tick) {
    return std::floor(value / tick) * tick;
}
inline double ceil_by_tick(double value, double tick) {
    return std::ceil(value / tick) * tick;
}
inline long long now_ms() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
