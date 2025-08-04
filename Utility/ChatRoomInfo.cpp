#include "ChatRoomInfo.h"

ChatRoomInfo::ChatRoomInfo()
{

}
void ChatRoomInfo::addMessage(const MsgInfo &msg)
{
    messages.push_back(msg);
    msg_count++;
    if(msg_count > 20) {
        messages.pop_front(); // Keep only the last 100 messages
        msg_count--;
    }
}
void ChatRoomInfo::addUser(const User &user)
{
    users.push_back(user);
    user_count++;
}
void ChatRoomInfo::removeUser(const int uid)
{
    // Assuming user_name is actually user_id, as per User class
    for(int i = 0; i < user_count; i ++)
    {
        if(users[i].Getuserid() == uid)
        {
            users.erase(users.begin() + i);
            user_count--;
            return;
        }
    }
}

std::vector<User> ChatRoomInfo::getUsers() const
{
    return users;
}

int ChatRoomInfo::getUserCount() const {
    return users.size();
}

std::string ChatRoomInfo::MsgsToJson()
{
    std::string MsgsJson = "{";
    for(int i = messages.size() - 10; i < messages.size(); i ++)
    {
        MsgsJson += messages[i].toJson();
    }
    MsgsJson += "}";
    return MsgsJson;
}
