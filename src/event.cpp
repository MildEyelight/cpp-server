#include "event.h"
#include <cstring>
#include <sys/socket.h>
#include <cstdio>
#include <unistd.h>
#include <iostream>

#include "Socket.h"

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

static int getline(char* buf, int size, FILE* fp){
    fgets(buf, size, fp);
    buf[strcspn(buf, "\n")] = '\0';
    return 0;
}

int client_event(const Socket& sock){
    return client_event(sock.get_sockfd());
}

int client_event(int sockfd){
    char buf[MAX_BUFFER_SIZE];
    memset(buf, 0, sizeof(buf));
    //scanf("%s", buf);
    getline(buf, sizeof(buf), stdin);
    ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
    if(write_bytes == -1){          //write返回-1，表示发生错误
        printf("socket already disconnected, can't write any more!\n");
        return write_bytes;
    }
    memset(buf, 0, sizeof(buf));
    ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
    if(read_bytes == -1){
        close(sockfd);
        printf("Read Server Error\n");
    }
    else if(read_bytes == 0){
        printf("server socket disconnected!\n");
    }
    else{
        printf("server: %s\n", buf);
    }
    return read_bytes;
}