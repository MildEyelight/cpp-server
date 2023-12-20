#include "Server.h"
#include "event.h"
#include "Channel.h"
#include <vector>
#include <functional>

EventLoop::EventLoop(){
    ep = std::make_shared<Epoll>();
}
EventLoop::~EventLoop(){
}
void EventLoop::loop(){
    while(true){
        std::vector<Channel*> activate_channels = ep->wait();
        for(Channel* channel: activate_channels){
            channel->handle_event();
        }
    }
}

int EventLoop::update_channel(Channel* channel){
    return ep->update_channel(channel);
}

std::shared_ptr<Epoll> EventLoop::get_epoll() const{
    return ep;
}

Server::Server(std::shared_ptr<EventLoop>& loop): loop(loop){
    acceptor = std::make_shared<class Acceptor>(loop.get());
    std::function<void(Socket*)> cb = std::bind(&Server::new_connection, this, std::placeholders::_1);
    acceptor->set_cb(cb);
}
Server::~Server(){
}
int Server::handle_read_event(int client_sockfd){

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

void Server::new_connection(Socket *server_socket){
    Socket* listen_socket = server_socket;
    Socket* client_socket = new Socket(listen_socket->accept());
   // Socket client_socket = listen_socket.accept();
   client_socket->set_non_block();
   Channel* new_client_channel = new Channel(loop.get(), client_socket->get_sockfd());
   std::function<void()> cb = std::bind(&Server::handle_read_event, this, new_client_channel->get_fd());
   new_client_channel->set_callback(cb);
   new_client_channel->enable_reading();
}


Acceptor::Acceptor(EventLoop* loop):loop(loop){
    listen_socket = new Socket();
    server_address = new InetAddress(SERVER_IP, SERVER_PORT);
    listen_socket->bind(*server_address);
    listen_socket->listen();
    listen_socket->set_non_block();
    accept_channel = new Channel(loop, listen_socket->get_sockfd());
    std::function<void()> cb = std::bind(&Acceptor::accept_connection, this);
    accept_channel->set_callback(cb);
    accept_channel->enable_reading();
}

void Acceptor::accept_connection(){
    this->new_connection_cb(listen_socket);
}