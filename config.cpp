#include "config.h"

Config::Config(){
    //端口号,默认8000 -p
    PORT = 8000;

    //日志写入方式，默认同步 -l
    LOGWrite = 0;

    //触发组合模式,默认listenfd LT + connfd LT  -m
    TRIGMode = 0;

    //listenfd触发模式，默认LT
    LISTENTrigmode = 0; 

    //connfd触发模式，默认LT
    CONNTrigmode = 0;

    //优雅关闭链接，默认不使用
    OPT_LINGER = 0;

    //数据库连接池数量,默认8
    sql_num = 8;

    //线程池内的线程数量,默认8
    thread_num = 8;

    //关闭日志,默认不关闭
    close_log = 0;

    //并发模型,默认是proactor
    actor_model = 0;
}

void Config::parse_arg(int argc, char*argv[]){
    int opt;
    const char *str = "p:l:o:s:t:c:";
    //const char* str = "p:l:m:o:s:t:c:a:";
    while ((opt = getopt(argc, argv, str)) != -1)
    {
        switch (opt)
        {
        case 'p'://端口号
        {
            PORT = atoi(optarg);
            break;
        }
        case 'l'://日志写入方式同步 异步
        {
            LOGWrite = atoi(optarg);
            break;
        }
        case 'm':
        {
            TRIGMode = atoi(optarg);
            break;
        }
        case 'o'://是否启用优雅关闭链接
        {
            OPT_LINGER = atoi(optarg);
            break;
        }
        case 's'://数据库连接池连接个数
        {
            sql_num = atoi(optarg);
            break;
        }
        case 't'://线程池线程个数
        {
            thread_num = atoi(optarg);
            break;
        }
        case 'c'://是否关闭日志 默认不关闭
        {
            close_log = atoi(optarg);
            break;
        }
        case 'a':
        {
            actor_model = atoi(optarg);
            break;
        }
        default:
            break;
        }
    }
}