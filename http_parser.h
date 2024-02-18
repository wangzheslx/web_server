#ifndef _HTTP_PARSER_H
#define _HTTP_PARSER_H
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <poll.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <sys/uio.h>

class http_parser{
    public:
    static const int FILENAME_LEN = 200;
    static const int READ_BUFFER_SIZE = 2048;
    static const int WRITE_BUFFER_SIZE = 1024;
    //HTTP的请求方法
    enum METHOD
    {
        GET = 0,
        POST,
        HEAD,
        PUT,
        DELETE,
        TRACE,
        OPTIONS,
        CONNECT
    };
    //主状态机的状态
    enum CHECK_STATE
    {
        CHECK_STATE_REQUESTLINE = 0,//状态0，表示当前正在分析请求行
        CHECK_STATE_HEADER,//状态1，表示当前正在分析请求头
        CHECK_STATE_CONTENT////状态2，表示当前正在分析请求正文
    };
    //解析结果
    enum HTTP_CODE
    {
        NO_REQUEST,
        GET_REQUEST,
        BAD_REQUEST,
        NO_RESOURCE,
        FORBIDDEN_REQUEST,
        FILE_REQUEST,
        INTERNAL_ERROR,
        CLOSED_CONNECTION
    };
    //从状态机的状态
    enum LINE_STATUS
    {
        LINE_OK = 0,
        LINE_BAD,
        LINE_OPEN
    };
    
    public:
    http_parser();
    ~http_parser();
    void init(int socket, char* root);//当前对象的初始化
    void init();//当前对象的初始化的重载
    void addfd(int epfd, int fd, uint32_t events);
    void read_once();//执行一次读操作
    
    LINE_STATUS parse_line();//解析一行
    char *get_line() { return m_read_buf + m_start_line; };//返回当前行字符串
    HTTP_CODE parse_request_line(char *text);//分析请求行
    HTTP_CODE parse_headers(char *text);//分析请求头
    HTTP_CODE parse_content(char *text);//分析请求正文
    HTTP_CODE do_request();//处理请求

    void process();
    HTTP_CODE process_read();//分析读到的数据
    bool process_write(HTTP_CODE ret);
    
    bool add_response(const char *format, ...);
    bool add_content(const char *content);
    bool add_status_line(int status, const char *title);
    bool add_headers(int content_length);
    bool add_content_type();
    bool add_content_length(int content_length);
    bool add_linger();
    bool add_blank_line();
    void close_conn(bool real_close = true);
    void removefd(int epollfd, int fd);
    void unmap();
    bool do_write();
    public:
    int m_socket;//描述当前连接的文件描述符
    CHECK_STATE m_check_state;//主状态机的状态
    sockaddr_in address;//当前连接对端的地址信息
    
    static int m_user_count;//类静态变量，当前连接的客户端数量
    static int m_epollfd;//epoll句柄的文件描述符
    int data_read = 0;			//接受read的返回值
    int m_read_idx = 0;			//当前已经读取了多少个字节的客户数据(data_read的累加)
    int m_write_idx = 0;        //m_write_buf中有效数据的大小
    int m_checked_idx = 0;		//当前已经分析完了多少个字节的客户数据
    int m_start_line = 0;			//行在buffer中的起始位置
    char *m_url;//当前请求的URL
    char *m_version;//当前请求的HTTP版本信息
    METHOD m_method;//当前请求的请求方法
    int m_content_length;//当前请求的"Connection:"字段的值
    bool m_keep_alive;//当前请求的"keep-alive"字段的值
    char *m_host;//当前请求的"Host:"字段的值
    char *m_string;//存储HTTP的请求数据
    
    char *server_root;//服务器资源的根路径
    struct stat m_file_stat;
    char* m_file_address;   //请求资源的映射地址
    struct iovec m_iv[2];
    /*
    ssize_t writev(int fd, const struct iovec *iov, int iovcnt);
    struct iovec {
               void  *iov_base;    //Starting address
               size_t iov_len;     //Number of bytes to transfer
           };*/
    int m_iv_count;
    int bytes_to_send;//响应头+响应正文的总长度
    private:
    char m_read_buf[ READ_BUFFER_SIZE ];//读缓冲
    char m_write_buf[ WRITE_BUFFER_SIZE ];//写缓冲
    char m_real_file[FILENAME_LEN];//请求的资源在服务器中的真实路径名
}; 
#endif