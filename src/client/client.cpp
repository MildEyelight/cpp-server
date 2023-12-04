#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <string>

const int MAX_BUFFER_SIZE = 1024;
const int SERVER_PORT = 11111;
const char* SERVER_IP = "127.0.0.1";

int getline(char* buf, int size, FILE* fp){
    fgets(buf, size, fp);
    buf[strcspn(buf, "\n")] = '\0';
    return 0;
}

int main(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serv_addr.sin_port = htons(SERVER_PORT);

    //连接服务器
    int rc = 0;
    rc = connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
    if(rc == -1){
        //可能是ECONNREFUSED 或者 ETIMEDOUT
        printf("Connect Server Error");
        exit(-1);
    }
    printf("Connect Server Successfully fd:%d, server IP: %s, port: %d\n", sockfd, 
                inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));
    
    while(true){
        char buf[MAX_BUFFER_SIZE];
        memset(buf, 0, sizeof(buf));
        //scanf("%s", buf);
        getline(buf, sizeof(buf), stdin);
        ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
        if(write_bytes == -1){          //write返回-1，表示发生错误
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        memset(buf, 0, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if(read_bytes == -1){
            close(sockfd);
            printf("Read Server Error\n");
            exit(-1);
        }
        else if(read_bytes == 0){
            printf("server socket disconnected!\n");
            break;
        }
        else{
            printf("server: %s\n", buf);
        }
    }
    return 0;
}