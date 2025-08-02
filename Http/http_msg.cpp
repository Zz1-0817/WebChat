#include "http_msg.h"

using namespace HTTP;

#include <iostream>
#include <string>
 
bool ends_with(const std::string& str, const std::string& suffix) {
    if (suffix.size() > str.size()) return false;
    return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool start_with(const std::string& str, const std::string& suffix) {
    if (suffix.size() > str.size()) return false;
    return str.substr(0, suffix.size()) == suffix;
}

//返回参数中文件名的类型
std::string GetContentType(const std::string& filename) {
    if (ends_with(filename, ".html")) return "text/html";
    if (ends_with(filename, ".css")) return "text/css";
    if (ends_with(filename, ".js")) return "application/javascript";
    if (ends_with(filename, ".png")) return "image/png";
    if (ends_with(filename, ".jpg") || ends_with(filename, ".jpeg")) return "image/jpeg";
    // 其他类型可按需添加
    return "application/octet-stream";
}

//读取文件内容
std::string ReadFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) return "";
    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
}

HttpMsg::HttpMsg(std::string HR)
{
    // 解析 HTTP 请求字符串 HR
    if (HR.empty()) {
        printf("http请求为空\n");
        return;
    }
    //分割HR为headers和body部分
    size_t pos = HR.find("\r\n\r\n");
    std::string header_part = HR.substr(0, pos);
    body = HR.substr(pos + 4); // 4 是 \r\n\r\n 的长度
    std::istringstream header_stream(header_part);// 将头部部分转换为流
    std::string request_line;
    std::getline(header_stream, request_line); // 第一行为请求行
    std::istringstream request_line_stream(request_line);
    // 解析请求行
    std::string method_str;
    request_line_stream >> method_str >> uri;// 获取请求方法与URI

    // 解析头部信息
    std::string line;
    while (std::getline(header_stream, line)) {
        if (line.empty() || line == "\r") continue;
        size_t sep = line.find(':');
        if (sep != std::string::npos) {
            std::string key = line.substr(0, sep);
            std::string value = line.substr(sep + 1);
            // 去除 value 前后的空格和回车

            value.erase(0, value.find_first_not_of(" \r"));
            value.erase(value.find_last_not_of(" \r") + 1);
            this -> headers[key] = value;
        }
    }
}
