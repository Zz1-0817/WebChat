#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "Socket.h"
#include "http_handle.h"

#define MAX_EVENTS 1024
#define PORT 18080


// 处理请求

int main() {
    Socket m_socket;
    ChatRoomInfo m_ChatRoomInfo;
    SqlHandle m_sqlHandle("chat.db");


    m_socket.bind("127.0.0.1",18080);

    m_socket.listen(MAX_EVENTS);

    int epfd = epoll_create(MAX_EVENTS);
    epoll_event ev, events[MAX_EVENTS];
    ev.data.fd = m_socket.get_fd();
    ev.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, m_socket.get_fd(), &ev);

    while (true) {
        int n = epoll_wait(epfd, events, MAX_EVENTS, -1);
        for (int i = 0; i < n; ++i) {
            int fd = events[i].data.fd;
            if (fd == m_socket.get_fd()) {
                // 新连接
                int conn_fd = m_socket.accept();
                ev.data.fd = conn_fd;
                ev.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_ADD, conn_fd, &ev);
            } else if (events[i].events & EPOLLIN) {
                char buf[4096] = {0};
                int len = recv(fd, buf, sizeof(buf), 0);
                if (len <= 0) {
                    close(fd);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, nullptr);
                } else {
                    std::string req(buf, len);
                    HttpHandle hhandle(req, &m_sqlHandle, &m_ChatRoomInfo, fd);
                    hhandle.HandleRoute();
                    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, nullptr);
                }
            }
        }
    }
    return 0;
}
