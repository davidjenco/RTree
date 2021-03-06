#pragma once

#include <chrono>
#include <assert.h>

//https://stackoverflow.com/questions/2808398/easily-measure-elapsed-time
template <class DT = std::chrono::nanoseconds,
        class ClockT = std::chrono::high_resolution_clock>
class Timer
{
    using timep_t = typename ClockT::time_point;
    timep_t _start = ClockT::now(), _end = {};

public:
    void tick() {
        _end = timep_t{};
        _start = ClockT::now();
    }

    void tock() { _end = ClockT::now(); }

    template <class T = DT>
    auto duration() const {
        assert(_end != timep_t{} && "toc before reporting");
        return std::chrono::duration_cast<T>(_end - _start);
    }
};


