#ifndef WEB_SERVER_H
#define WEB_SERVER_H
#include "http_parser.h"
#include "threadpool.h"
#define MAX_EVENT_NUMBER 2000 //监听事件的最大个数
#define MAX_FD 2000 //文件描述符上限
class web_server{
    public:
    web_server();
    ~web_server();
    void init(int port, int threadnumber);
    void event_listen();
    void event_Loop(); 
    void deal_new_connection();
    void deal_read_event(int socket);
    void addfd(int epfd, int fd, uint32_t events);
    public:
    int m_listenfd;//监听套接字的文件描述符
    int m_port;//我们监听的端口号
    int m_epollfd;//描述epoll的文件描述符
    char *m_root;//服务器的资源根目录的路径
    http_parser* users;//客户端数组
    epoll_event events[ MAX_EVENT_NUMBER ];//epoll_wait传参用
    //线程池
    void thread_pool();
    threadpool<http_parser> *m_pool;
    int m_thread_num;

};
#endif