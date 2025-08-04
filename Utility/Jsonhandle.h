#pragma once

#include <string>
#include <map>

class Jsonhandle {
public:
    std::map<std::string, std::string> Parse(const std::string &jsonString);
    std::string ReParse(const std::map<std::string, std::string> &jsonMap);
};
