#include "TimeH.h"

void TimeH::GetTime() {
    time_t ticks = time(NULL);
    struct tm* ptm = localtime(&ticks);
    char buffer[80];
    memset(buffer, 0, sizeof(buffer));
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ptm);
    time_str = std::string(buffer);
}

std::string TimeH::GetTimeStr() {
    return time_str;
}