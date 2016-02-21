#pragma once

#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <string>

namespace Timer {
    std::string now() {
        auto now = std::chrono::system_clock::now();
        auto now_in_time_t = std::chrono::system_clock::to_time_t(now);

        std::stringstream ss;
        ss << std::put_time(std::localtime(&now_in_time_t), "%Y%m%d%H%I%S");

        return ss.str();
    }
}