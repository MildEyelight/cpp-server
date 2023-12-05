#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <iostream>
#include <memory>

#include "Socket.h"
#include "InetAddress.h"
#include "Epoll.h"
#include "event.h"

const int MAX_BUFFER_SIZE = 1024;
const int SERVER_PORT = 11111;
const char* SERVER_IP = "127.0.0.1";
//const int MAX_EVENTS = 128;

void errif(bool condition, const char* err_msg){
    if(condition){
        perror(err_msg);
        exit(EXIT_FAILURE);
    }
}




int main(){
    
    // 1. 打开socket 绑定端口
    int rc = 0;
    Socket listen_socket;
    InetAddress server_address(SERVER_IP, SERVER_PORT);
    rc = listen_socket.bind(server_address);
    errif(rc == -1, "Bind Socket Error");
    rc = listen_socket.listen();
    errif(rc == -1, "Listen Socket Error");

    int sockfd = listen_socket.get_sockfd();
    // 2.5 创建epoll
    Epoll listen_epoll;
    listen_epoll.add_socket(listen_socket);


    // 3. 从监听队列取出连接请求->从epoll中取出就绪的文件描述符
    while(true){
        int fd_nums = listen_epoll.wait();
        auto events = listen_epoll.get_events();
        for(int i = 0; i < fd_nums; i++){
            if(events[i].data.fd == listen_socket.get_sockfd()){
                Socket* client_socket = new Socket(listen_socket.accept());
                // Socket client_socket = listen_socket.accept();
                client_socket->set_non_block();
                listen_epoll.add_socket(client_socket, EPOLLIN | EPOLLET);
            }
            else{
                handle_event(events[i].data.fd);
            }
        }
    }
    return 0;
}
