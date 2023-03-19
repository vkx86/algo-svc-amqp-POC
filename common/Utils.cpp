//
// Created by developer on 1/9/22.
//

#include <chrono>
#include "Utils.h"

std::string Utils::getCurrTime() {
    auto time = std::chrono::system_clock::now().time_since_epoch();
    std::chrono::seconds seconds = std::chrono::duration_cast< std::chrono::seconds >(time);
    std::chrono::microseconds ms = std::chrono::duration_cast< std::chrono::microseconds >(time);
    auto result = (double) seconds.count() + ((double) (ms.count() % 1000000)/1000000.0);
    return std::to_string(result);
}

//based on: https://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/
timespec Utils::timespec_diff(const timespec& start, const timespec& end) {
    timespec result{};
    if ((end.tv_nsec - start.tv_nsec) < 0) {
        result.tv_sec = end.tv_sec - start.tv_sec - 1;
        result.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else {
        result.tv_sec = end.tv_sec - start.tv_sec;
        result.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return result;
}
