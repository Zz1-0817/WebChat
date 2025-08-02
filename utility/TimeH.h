#pragma once

#include <time.h>
#include <string.h>
#include <string>

class TimeH 
{
    public:
        std::string GetTimeStr();
    private:
        void GetTime();
        TimeH() { GetTime(); }
    protected:
        std::string time_str;
};