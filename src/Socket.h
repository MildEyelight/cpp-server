#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include "InetAddress.h"
#include "error.h"

class Socket{
// 希望一个Sokcet对象能够唯一的维护一个file discriptor
private:
    int _sockfd;

public:
    Socket(){
        _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    }
    Socket(Socket&& a){
        _sockfd = a._sockfd;
        a._sockfd = -1;
    }
    Socket(const Socket& a) = delete;
    Socket(int sockfd): _sockfd(sockfd){}
    
    ~Socket(){
        close(_sockfd);
    }
    
    int get_sockfd() const{
        return _sockfd;
    } 
    void set_non_block(){
        fcntl(_sockfd, F_SETFL, fcntl(_sockfd, F_GETFL) | O_NONBLOCK);
    }
    int bind(const InetAddress& server_address){
        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));// 初始化对象

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(server_address.get_ip().c_str());
        serv_addr.sin_port = htons(server_address.get_port());

        return ::bind(_sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)); // 将套接字fd绑定到端口
    }
    int listen(){
        return ::listen(_sockfd, SOMAXCONN);
    }
    Socket accept(){
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_sockfd = ::accept(_sockfd, (sockaddr*)&client_addr, &client_addr_len); // accept 会发生阻塞。
        errif(client_sockfd == -1, "Accept Socket Error\n");
        printf("New client fd %d, IP: %s, Port: %d\n", client_sockfd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port)); 
    
        return Socket(client_sockfd);
    }

    int connect(const InetAddress& server_address){
        struct sockaddr_in serv_addr;
        bzero(&serv_addr, sizeof(serv_addr));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(server_address.get_ip().c_str());
        serv_addr.sin_port = htons(server_address.get_port());
        return ::connect(_sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
    }
};