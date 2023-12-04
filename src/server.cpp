#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <iostream>

const int MAX_BUFFER_SIZE = 1024;
const int SERVER_PORT = 11111;
const char* SERVER_IP = "127.0.0.1";
const int MAX_EVENTS = 128;

void errif(bool condition, const char* err_msg){
    if(condition){
        perror(err_msg);
        exit(EXIT_FAILURE);
    }
}

int handle_event(int client_sockfd){
    char buf[MAX_BUFFER_SIZE];
    bzero(buf, sizeof(buf));
    ssize_t read_bytes = read(client_sockfd, buf, sizeof(buf));
    if(read_bytes > 0){
        printf("client: %s\n", buf);
        write(client_sockfd, buf, sizeof(buf));
        return 1;
    }
    else if(read_bytes == 0){
        close(client_sockfd);
        printf("Client disconnect\n");
        return 0;
    }
    else{
        close(client_sockfd);
        printf("Error reading\n");
        return -1;
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
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serv_addr.sin_port = htons(SERVER_PORT);

    rc = bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)); // 将套接字fd绑定到端口
    errif(rc == -1, "Bind Socket Error");

    // 2. 开启监听
    rc = listen(sockfd, SOMAXCONN);
    errif(rc == -1, "Listen Socket Error");
    
    // 2.5 创建epoll
    int epfd = epoll_create(1024);
    errif(epfd == -1, "Creating Epoll Error");

    struct epoll_event events[MAX_EVENTS], ev;
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

    // 3. 从监听队列取出连接请求->从epoll中取出就绪的文件描述符
    while(true){
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        for(int i = 0; i < nfds; i++){
            if(events[i].data.fd == sockfd){
                struct sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                int client_sockfd = accept(sockfd, (sockaddr*)&client_addr, &client_addr_len); // accept 会发生阻塞。
                if((rc = client_sockfd) == -1, "Accept Socket Error\n");
                printf("New client fd %d, IP: %s, Port: %d\n", client_sockfd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                ev.data.fd = client_sockfd;
                ev.events = EPOLLIN | EPOLLET;
                fcntl(client_sockfd, F_SETFL, fcntl(client_sockfd, F_GETFL) | O_NONBLOCK);
                epoll_ctl(epfd, EPOLL_CTL_ADD, client_sockfd, &ev);
            }
            else if(events[i].events & EPOLLIN){
                handle_event(events[i].data.fd);
            }
        }
    }
    // struct sockaddr_in client_addr;
    // socklen_t client_addr_len = sizeof(client_addr);
    // memset(&client_addr, 0, sizeof(client_addr));
    // sleep(20);
    // int client_sockfd = accept(sockfd, (sockaddr*)&client_addr, &client_addr_len); // accept 会发生阻塞。
    // if((rc = client_sockfd) == -1, "Accept Socket Error\n");
    // printf("New client fd %d, IP: %s, Port: %d\n", client_sockfd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    
    // while(true){
    //     //sleep(10);
    //     //这里写建立连接后的接受处理程序
    // }
    return 0;
}
