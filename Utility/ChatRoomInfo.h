#pragma once


#include "UserInfo.h"
#include <string>
#include "MsgInfo.h"
#include <vector>
#include <queue>

class ChatRoomInfo 
{
    public:
        ChatRoomInfo();

        void addMessage(const MsgInfo &msg);
        void addUser(const User &user);
        void removeUser(const int userid);
        std::vector<User> getUsers() const;
        int getUserCount() const;
        std::string MsgsToJson();

    private:
        std::deque<MsgInfo> messages;
        std::vector<User> users;
        int user_count;
        int msg_count;
};
