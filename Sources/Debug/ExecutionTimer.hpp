//////////////////////////////
///// ExecutionTimer.hpp /////
//////////////////////////////

#pragma once
#include "../Libraries.hpp"

class ExecutionTimer
{
public:
    ExecutionTimer() noexcept
    {
        start = std::chrono::steady_clock::now();
    }

    ~ExecutionTimer() noexcept
    {
        end = std::chrono::steady_clock::now();

        auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        std::clog << "Time taken : " << microseconds << " Microseconds (" << milliseconds << " Milliseconds)\n";
    }

    ExecutionTimer(const ExecutionTimer&) = delete;
    ExecutionTimer& operator=(const ExecutionTimer&) = delete;
    ExecutionTimer(ExecutionTimer&&) = delete;
    ExecutionTimer& operator=(ExecutionTimer&&) = delete;

private:
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;
};
