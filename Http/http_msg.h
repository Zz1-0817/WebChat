#pragma once

#include <unistd.h>
#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <algorithm>

namespace HTTP
{
    #define MAX_EVENTS 1024
    #define PORT 18080
    class HttpMsg
    {
        public:
            enum Method 
            {
                GET = 0, POST, PUT, DELETE, UNKNOWN 
            }method;
            std::string uri;
            std::map<std::string, std::string> headers;
            std::string body;
            void setMethod(const std::string& method_str) {
                if (method_str == "GET") method = GET;
                else if (method_str == "POST") method = POST;
                else if (method_str == "PUT") method = PUT;
                else if (method_str == "DELETE") method = DELETE;
                else method = UNKNOWN;
            }
            HttpMsg(std::string HR){}
        private:
            HttpMsg() = default;
    };
    bool ends_with(const std::string& str, const std::string& suffix) {
    if (suffix.size() > str.size()) return false;
    return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool start_with(const std::string& str, const std::string& suffix);

//返回参数中文件名的类型
std::string GetContentType(const std::string& filename);
//读取文件内容
std::string ReadFile(const std::string& filename);
}