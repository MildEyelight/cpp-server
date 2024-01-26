#include "Connection.h"
#include "event.h"
Connection::Connection(EventLoop* _loop, Socket* _sock): loop(_loop), sock(_sock) {
    channel = std::make_shared<Channel>(loop, sock->get_sockfd());
    std::function<void()> cb = std::bind(&Connection::echo, this, sock->get_sockfd());
    channel->set_callback(cb);
    channel->enable_reading();
}

Connection::~Connection(){
    delete sock;
}

int Connection::disconnect(){
    return disconnect_cb(this->sock->get_sockfd());
}

int Connection::send_message(){
    return write(sock->get_sockfd(), read_buffer.c_str(), read_buffer.size());
}
int Connection::clear_buffer(){
    return read_buffer.clear();
}
int Connection::echo(const int client_sockfd){
    char buf[MAX_BUFFER_SIZE];
    while(true){
        bzero(buf, sizeof(buf));
        ssize_t read_bytes = read(client_sockfd, buf, sizeof(buf));
        if(read_bytes > 0){
            read_buffer.append(buf, read_bytes);
        }
        else if(read_bytes == -1 && errno == EINTR){
            printf("Client dissconnect, continue reading\n");
            continue;
        }
        else if(read_bytes == -1 && ((errno == EAGAIN) || errno == EWOULDBLOCK)){
            printf("Message from client fd %d: %s\n", client_sockfd, read_buffer.c_str());
            int rc = this->send_message();
            if(rc == -1){
                printf("socket write error\n");
                this->disconnect();
                break;
            }
            this->clear_buffer();
            break;
        }
        else if(read_bytes == 0){
            printf("Client disconnected !\n");
            this->disconnect();
            break;
        }
        else{
            printf("Unknown error, disconnect!\n");
            this->disconnect();
            break;
        }
    }
    return 0;
}