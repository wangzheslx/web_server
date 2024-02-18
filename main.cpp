#include "web_server.h"
//主函数传一个端口号
int main(int argc, char* argv[]){
    
    if(argc != 2){
        printf("请输入正确的参数\n");
        exit(1);
    }
    
    int port = atoi(argv[1]);
    web_server server;
    server.init(port);
    server.event_listen();
    server.event_Loop();
    return 0;
}