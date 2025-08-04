#pragma once

#include "http_msg.h"
#include "UserInfo.h"
#include <vector>
#include "SQLhandle.h"
#include "ChatRoomInfo.h"
#include <string>
#include <sys/socket.h>

class HttpHandle
{
    public:
        HttpHandle() = default;
        HttpHandle(const std::string H_string, SqlHandle *sqlHandle, ChatRoomInfo *Cri, int fd);
        void HandleRoute() const;
        void HandleResponse() const;
        void HandleRequest() const;
        void HandleGetRequest() const;
        void HandlePostRequest() const;
        void Handlesendresponse(const std::string &response) const;
        void HandleLogin() const;
        void HandleLogout() const;
        void HandleRegister() const;
        void Handlesendmsg() const;
    private:
        HttpMsg httpMsg;
        User *mUser;
        SqlHandle *mSqlHandle;
        ChatRoomInfo *m_Cri;
        int m_fd;
};

