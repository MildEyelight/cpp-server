#include "Connection.h"
#include "event.h"
Connection::Connection(EventLoop* _loop, Socket* _sock): loop(_loop), sock(_sock) {
    channel = std::make_shared<Channel>(loop, sock->get_sockfd());
    std::function<void()> cb = std::bind(&Connection::echo, this, sock->get_sockfd());
    channel->set_callback(cb);
    channel->enable_reading();
}
Connection::~Connection(){
}

int Connection::echo(const int client_sockfd){
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