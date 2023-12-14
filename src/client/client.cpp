#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <string>

#include "src/Socket.h"
#include "src/InetAddress.h"
#include "src/event.h"

const int SERVER_PORT = 11111;
const char* SERVER_IP = "127.0.0.1";

int main(){
    Socket client_socket;
    InetAddress server_address(SERVER_IP, SERVER_PORT);
    int rc = client_socket.connect(server_address);
    if(rc != -1){
        printf("Connect to server successfully. Server ip: %s, server port: %d\n", SERVER_IP, SERVER_PORT);
    }
    else{
        //可能是ECONNREFUSED 或者 ETIMEDOUT
        printf("Connect Server Error\n");
        exit(-1);
    }

    while(true){
        rc = client_event(client_socket);
        if(rc == -1){
            return -1;
        }
        else if(rc == 0){
            break;
        }
    }
    return 0;
}