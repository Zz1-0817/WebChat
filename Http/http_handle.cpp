#include "http_handle.h"

using namespace HTTP;

HttpHandle::HttpHandle(std::string H_string) : httpMsg(H_string), mUser(nullptr) {
    // Initialize HttpMsg with the provided HTTP request string
}

void HttpHandle::HandleRoute() const {
    switch(httpMsg.method) {
        case 0:
            HandleGetRequest();
            // Handle GET request
            break;
        case 1:
            HandlePostRequest();
            // Handle POST request
            break;
        case 2:
            HandlePutRequest();
            // Handle PUT request
            break;
        case 3:
            HandleDeleteRequest();
            // Handle DELETE request
            break;
        default:
            // Handle unknown method
            break;
    }
}

void HttpHandle::HandleGetRequest() const
{
    std::string m_uri = "public";
    if(httpMsg.uri == "/")m_uri += "/index.html";
    else m_uri += httpMsg.uri;
    std::string file_content = HTTP::ReadFile(m_uri);

    if (!file_content.empty()) {
        std::string content_type = GetContentType(m_uri);
        std::ostringstream response;
        response << "HTTP/1.1 200 OK\r\n";
        response << "Content-Type: " << content_type << "\r\n";
        response << "Content-Length: " << file_content.size() << "\r\n";
        response << "Connection: close\r\n\r\n";
        response << file_content;
        Handlesendresponse(response.str());
    } else {
        std::string not_found = "<h1>404 Not Found</h1>";
        std::ostringstream response;
        response << "HTTP/1.1 404 Not Found\r\n";
        response << "Content-Type: text/html\r\n";
        response << "Content-Length: " << not_found.size() << "\r\n";
        response << "Connection: close\r\n\r\n";
        response << not_found;
        Handlesendresponse(response.str());
    }
}

void HttpHandle::HandlePostRequest() const
{
    if(httpMsg.uri == "login")//用户登录
    {

    }
    else if(httpMsg.uri == "logout")//用户登出
    {

    }
    else if(httpMsg.uri == "register")//用户注册
    {

    }
    else if(httpMsg.uri == "sendmsg")
    {
        
    }
}

void HttpHandle::Handlesendresponse(const std::string &m_file) const
{

}