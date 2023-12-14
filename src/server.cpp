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
#include "error.h"
const int MAX_BUFFER_SIZE = 1024;
const int SERVER_PORT = 11111;
const char* SERVER_IP = "127.0.0.1";
//const int MAX_EVENTS = 128;



int main(){
    
    // 1. 打开socket 绑定端口
    int rc = 0;
    Socket listen_socket;
    InetAddress server_address(SERVER_IP, SERVER_PORT);
    rc = listen_socket.bind(server_address);
    errif(rc == -1, "Bind Socket Error");
    rc = listen_socket.listen();
    errif(rc == -1, "Listen Socket Error");

    // 2.5 创建epoll
    Epoll listen_epoll;
    listen_socket.set_non_block();
    Channel listen_channel(&listen_epoll, listen_socket.get_sockfd());
    listen_channel.enable_reading();
    listen_epoll.add_socket(listen_socket);

    // 3. 从监听队列取出连接请求->从epoll中取出就绪的文件描述符
    while(true){
        std::vector<Channel*> active_channels = std::move(listen_epoll.wait());
        for(int i = 0; i < (int)active_channels.size(); i++){
            int channel_fd = active_channels[i]->get_fd();
            if(channel_fd == listen_socket.get_sockfd()){
                Socket* client_socket = new Socket(listen_socket.accept());
                // Socket client_socket = listen_socket.accept();
                client_socket->set_non_block();
                Channel* new_client_channel = new Channel(&listen_epoll, client_socket->get_sockfd());
                new_client_channel->enable_reading();
            }
            else if (active_channels[i]->get_events() & EPOLLIN){
                handle_event(channel_fd);
            }
            else{
                std::cout << "Do Something Else";
            } 
        }
    }
    return 0;
}
