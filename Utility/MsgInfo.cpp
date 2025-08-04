#include "MsgInfo.h"

MsgInfo::MsgInfo(int uid, int mid, std::string context)
    :msg_id(mid), user_id(mid), msg_content(context), send_time(TimeH().GetTimeStr()) {};

std::string MsgInfo::toJson() const {
    Jsonhandle jsonParser;
    std::map<std::string, std::string> msgMap;
    msgMap["msgid"] = std::to_string(msg_id);
    msgMap["userid"] = std::to_string(user_id);
    msgMap["content"] = msg_content;
    msgMap["time"] = send_time;
    return jsonParser.ReParse(msgMap);
}

MsgInfo::MsgInfo(std::string &jsonString)
    :send_time(TimeH().GetTimeStr())
{
    Jsonhandle jsonParser;
    std::map<std::string, std::string> msgMap = jsonParser.Parse(jsonString);
    msg_id = std::stoi(msgMap["msgid"]);
    user_id = std::stoi(msgMap["userid"]);
    msg_content = msgMap["content"];
}

std::map<std::string, std::string> MsgInfo::toMap() const {
    return {
        {"msgid", std::to_string(msg_id)},
        {"userid", std::to_string(user_id)},
        {"content", msg_content},
        {"time", send_time}
    };
}

MsgInfo::MsgInfo(std::map<std::string, std::string> &msgMap)
    :msg_id(std::stoi(msgMap["msgid"])),
    user_id(std::stoi(msgMap["userid"])),
    msg_content(msgMap["content"]),
    send_time(TimeH().GetTimeStr())
{}

void MsgInfo::setMsgId(int mid)
{
    msg_id = mid;
}