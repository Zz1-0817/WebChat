#pragma once

#include "http_msg.h"
#include "../utility/UserInfo.h"
#include <vector>

namespace HTTP
{
    class HttpHandle
    {
        public:
            HttpHandle() = default;
            HttpHandle(std::string H_string);
            void HandleRoute() const;
            void HandleResponse() const;
            void HandleRequest() const;
            void HandleGetRequest() const;
            void HandlePostRequest() const;
            void HandlePutRequest() const;
            void HandleDeleteRequest() const;
            void Handlesendresponse(const std::string &m_file) const;
        private:
            HttpMsg httpMsg;
            User *mUser;
    };
}
