#pragma once
#include <string>
#include "TimeH.h"
#include <vector>
#include "Jsonhandle.h"

class User
{  
    public:
        User(const std::string &name, int id);
        std::string toJson();
        std::map<std::string, std::string> toMap();
        int Getuserid(){return user_id;}
    protected:
        int user_id;
        std::string user_name;
        std::string psw;
};