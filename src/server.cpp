#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
void errif(bool condition, const char* err_msg){
    if(condition){
        perror(err_msg);
        exit(EXIT_FAILURE);
    }
}


int main(){
    
    // 1. 打开socket 绑定端口
    int rc = 0;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // system call: 
    errif((rc = sockfd) == -1, "Create Socket Error");

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));// 初始化对象

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(11111);

    rc = bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)); // 将套接字fd绑定到端口
    errif(rc == -1, "Bind Socket Error");

    // 2. 开启监听
    rc = listen(sockfd, SOMAXCONN);
    errif(rc == -1, "Listen Socket Error");

    // 3. 从监听队列取出连接请求
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    memset(&client_addr, 0, sizeof(client_addr));
    
    int client_sockfd = accept(sockfd, (sockaddr*)&client_addr, &client_addr_len); // accept 会发生阻塞。
    if((rc = client_sockfd) == -1, "Accept Socket Error");

    printf("New client fd %d, IP: %s, Port: %d\n", client_sockfd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    while(true){
        sleep(10);
    }
    
    return 0;
}