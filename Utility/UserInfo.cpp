#include "UserInfo.h"

User::User(const std::string &name, int id) : user_name(name), user_id(id)
{

}

std::string User::toJson()
{
    return "{\"user_id\": " + std::to_string(user_id) + ", \"user_name\": \"" + user_name + "\"}";
}

std::map<std::string, std::string> User::toMap()
{
    std::map<std::string, std::string> userMap;
    userMap["user_id"] = std::to_string(user_id);
    userMap["user_name"] = user_name;
    userMap["psw"] = psw;
    return userMap;
}