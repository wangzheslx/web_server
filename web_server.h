#ifndef WEB_SERVER_H
#define WEB_SERVER_H
#include "http_parser.h"

#define MAX_EVENT_NUMBER 2000
#define MAX_FD 2000
class web_server{
    public:
    web_server();
    ~web_server();
    void init(int port);
    void event_listen();
    void event_Loop();
    void deal_new_connection();
    void deal_read_event(int socket);
    void addfd(int epfd, int fd, uint32_t events);
    public:
    int m_listenfd;
    int m_port;
    int m_epollfd;
    char *m_root;
    http_parser* users;
    epoll_event events[ MAX_EVENT_NUMBER ];
};
#endif