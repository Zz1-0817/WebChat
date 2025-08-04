#include "Jsonhandle.h"

std::map<std::string, std::string> Parse(const std::string &jsonString)
{
    std::map<std::string, std::string> jsonMap;
    size_t start = 0;
    size_t end = 0;

    while ((start = jsonString.find('"', end)) != std::string::npos) {
        end = jsonString.find('"', start + 1);
        if (end == std::string::npos) break; // No closing quote found

        std::string key = jsonString.substr(start + 1, end - start - 1);
        start = jsonString.find(':', end);
        if (start == std::string::npos) break; // No colon found

        end = jsonString.find(',', start);
        if (end == std::string::npos) {
            end = jsonString.find('}', start); // Handle last element
        }

        std::string value = jsonString.substr(start + 1, end - start - 1);
        jsonMap[key] = value;

        if (end == std::string::npos) break; // No more elements
    }

    return jsonMap;
}

std::string ReParse(const std::map<std::string, std::string> &jsonMap)
{
    std::string jsonString = "{";
    for(const auto &pair : jsonMap) {
        jsonString += "\"" + pair.first + "\":\"" + pair.second + "\",\r\n";
    }
    jsonString += "}";
    return jsonString;
}