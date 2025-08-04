#pragma once
#include <string>
#include "TimeH.h"
#include <vector>
#include "Jsonhandle.h"

class MsgInfo 
{
    public:
        MsgInfo();
        MsgInfo(int uid, int mid, std::string context);
        MsgInfo(std::map<std::string, std::string> &msgMap);
        MsgInfo(std::string &jsonString);
        std::string toJson() const;
        std::map<std::string, std::string> toMap() const;
        void setMsgId(int mid);
        void set_send_time();
    private:
        int msg_id;
        int user_id;
        std::string msg_content;
        std::string send_time;
};