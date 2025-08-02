#pragma once
#include <string>
#include "TimeH.h"
#include <vector>

class User
{  
    public:
        int GetUserId() const { return user_id; }
        std::string GetUserName() const { return user_name; }
        const std::vector<std::string> &GetMsg() const { return msg; }
        std::string GetSendTime() { return t.GetTimeStr(); }
        void SetUserId(int id) { user_id = id; }
        void SetUserName(const std::string& name) { user_name = name; }
        bool check(const std::string& psw) const {
            return this->psw == psw;
        }
        void GetMsg();
    protected:
        int user_id;
        std::string user_name;
        std::vector<std::string> msg;
        std::string psw;
        TimeH t;
};