#include "http_handle.h"

HttpHandle::HttpHandle(const std::string H_string, SqlHandle *sqlHandle, ChatRoomInfo *m_Cri, int fd) 
    : httpMsg(H_string), mUser(nullptr), mSqlHandle(sqlHandle), m_Cri(m_Cri), m_fd(fd) {
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
            // Handle PUT request
            break;
        case 3:
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
    std::string file_content = ReadFile(m_uri);

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
    if(httpMsg.uri == "login")HandleLogin();
    else if(httpMsg.uri == "logout")HandleLogout();
    else if(httpMsg.uri == "register")HandleRegister();
    else if(httpMsg.uri == "sendmsg")Handlesendmsg();
}


void HttpHandle::HandleLogin() const
{
    std::string m_body = httpMsg.body;
    Jsonhandle jsonParser;
    std::map<std::string, std::string> jsonMap;
    std::vector<std::map<std::string, std::string>> qurayMap;
    jsonMap = jsonParser.Parse(m_body);
    qurayMap = mSqlHandle -> query("SELECT * FROM users WHERE username = '" + jsonMap["username"]);
    std::map<std::string, std::string> jsonMap2;
    if(qurayMap.size() == 0)
    {
        jsonMap2["status"] = "error";
        jsonMap2["message"] = "User not found";
        std::string json1 = jsonParser.ReParse(jsonMap2);
        std::string response = "HTTP/1.1 200\r\nContent-Type: application/json\r\nContent-Length: " + std::to_string(json1.size()) + "\r\n\r\n" + json1;
        Handlesendresponse(response);
        return;
    }
    else if(qurayMap[0]["psw"] != jsonMap["psw"])
    {
        jsonMap2["status"] = "error";
        jsonMap2["message"] = "Incorrect password";
        std::string json1 = jsonParser.ReParse(jsonMap2);
        std::string response = "HTTP/1.1 200\r\nContent-Type: application/json\r\nContent-Length: " + std::to_string(json1.size()) + "\r\n\r\n" + json1;
        Handlesendresponse(response);
        return;
    }
    else {
        jsonMap2["status"] = "success";
        jsonMap2["message"] = "Correct password";
        m_Cri->addUser(User(jsonMap["username"], stoi(qurayMap[0]["userid"])));
        std::string json1 = jsonParser.ReParse(jsonMap2);
        std::string response = "HTTP/1.1 200\r\nContent-Type: application/json\r\nContent-Length: " + std::to_string(json1.size()) + "\r\n\r\n" + json1;
        Handlesendresponse(response);
    }
}

void HttpHandle::HandleLogout() const
{
    m_Cri->removeUser(mUser->Getuserid());
    // Handle user logout logic here
    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 2\r\n\r\n";
    response += "{count: " + std::to_string(m_Cri->getUserCount()) + "}";
    Handlesendresponse(response);
}

void HttpHandle::HandleRegister() const
{
    std::string m_body = httpMsg.body;
    Jsonhandle jsonParser;
    std::map<std::string, std::string> jsonMap;
    jsonMap = jsonParser.Parse(m_body);
    
    // Check if user already exists
    auto existingUser = mSqlHandle->query("SELECT * FROM users WHERE username = '" + jsonMap["username"] + "'");
    if (!existingUser.empty()) {
        std::map<std::string, std::string> responseMap;
        responseMap["status"] = "error";
        responseMap["message"] = "Username already exists";
        std::string jsonResponse = jsonParser.ReParse(responseMap);
        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: " + std::to_string(jsonResponse.size()) + "\r\n\r\n" + jsonResponse;
        Handlesendresponse(response);
        return;
    }

    // Insert new user into the database
    mSqlHandle->insert("users", jsonMap);
    
    std::map<std::string, std::string> responseMap;
    responseMap["status"] = "success";
    responseMap["message"] = "User registered successfully";
    std::string jsonResponse = jsonParser.ReParse(responseMap);
    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: " + std::to_string(jsonResponse.size()) + "\r\n\r\n" + jsonResponse;
    Handlesendresponse(response);
}

void HttpHandle::Handlesendmsg() const
{
    std::string m_body = httpMsg.body;
    
    MsgInfo msg1(m_body);

    mSqlHandle->insert("msgs", msg1.toMap());

    msg1.setMsgId(mSqlHandle->last_insert_rowid());

    m_Cri->addMessage(msg1);
    std::string msgjson = m_Cri -> MsgsToJson();

    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: " + std::to_string(msgjson.size()) + "\r\n\r\n" + msgjson;
    Handlesendresponse(response);
}

void HttpHandle::Handlesendresponse(const std::string &response) const
{
    ::send(m_fd, response.c_str(), response.size(), 0);
}