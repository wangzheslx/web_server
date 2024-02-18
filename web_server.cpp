#include "web_server.h"
web_server::web_server(){
    users = new http_parser[MAX_FD];
    //root文件夹路径
    char server_path[200];
    //获取可执行程序的工作目录:/home/pointer2022/pointer/2209/obj/3day/web
    getcwd(server_path, 200);
    char root[6] = "/root";
    m_root = (char *)malloc(strlen(server_path) + strlen(root) + 1);
    strcpy(m_root, server_path);
    strcat(m_root, root);
    //mroot = /home/pointer2022/pointer/2209/obj/3day/web/root
}
web_server::~web_server(){
}

void web_server::init(int port){
    m_listenfd = -1;
    m_epollfd = -1;
    m_port = port;
    memset(events, '\0', sizeof(events));
}

void web_server::event_listen(){
    //初始化一个监听套接字
    m_listenfd = socket(AF_INET, SOCK_STREAM, 0);
    //检错
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(m_port);
    //设置端口复用
    int flag = 1;
    setsockopt(m_listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    int ret = bind(m_listenfd, (struct sockaddr *)&address, sizeof(address));
    if(ret == -1){
        perror("bind error");
        exit(1);
    }
    listen(m_listenfd, 64);
    //创建一个epoll对象
    m_epollfd = epoll_create(64);
    http_parser::m_epollfd = m_epollfd;
    addfd(m_epollfd, m_listenfd, EPOLLIN);
}

void web_server::event_Loop(){
    while (1)
    {
        int number = epoll_wait(m_epollfd, events, MAX_EVENT_NUMBER, -1);
        for (int i = 0; i < number; i++){
            int sockfd = events[i].data.fd;
            //处理新到的客户连接
            if (sockfd == m_listenfd){
                deal_new_connection();
            }
            //处理客户连接上接收到的数据
            else if (events[i].events & EPOLLIN){
                deal_read_event(sockfd);
            }
        }
    }
}

void web_server::deal_new_connection(){
    struct sockaddr_in client_address;
    socklen_t client_addrlength = sizeof(client_address);
    int connfd = accept(m_listenfd, (struct sockaddr *)&client_address, &client_addrlength);
    if (connfd < 0){
        //检错
    }
    if (http_parser::m_user_count >= MAX_FD){
        //报错
    }
    printf("新的客户端建立连接\n");
    users[connfd].init(connfd, m_root);
}

void web_server::deal_read_event(int socket){
    users[socket].read_once();
}

void web_server::addfd(int epfd, int fd, uint32_t events){
    epoll_event event;
    event.events = events;
    event.data.fd = fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
}