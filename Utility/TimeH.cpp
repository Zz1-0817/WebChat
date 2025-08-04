#include "TimeH.h"

std::string TimeH::GetTimeStr() const
{
    time_t ticks = time(NULL);
    struct tm* ptm = localtime(&ticks);
    char buffer[80];
    memset(buffer, 0, sizeof(buffer));
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ptm);
    return std::string(buffer);
}