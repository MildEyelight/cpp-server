#pragma once
#include "Epoll.h"
#include "Channel.h"
#include <memory>
class EventLoop{
private: 
    Epoll *ep;
    bool quit;
public:
    EventLoop() = default;
    void loop();
    int update_channel(Channel* channel);
    Epoll* get_epoll() const;
}
;

class Server{
private:
    //Reactor
    std::shared_ptr<EventLoop> loop;
public:
    Server(std::shared_ptr<EventLoop>& loop);
    ~Server();


    //Acceptor
    void new_connection(Socket *server_socket);
    //Handler
    int handle_read_event(int fd);
};

class Acceptor{
private:
    EventLoop* loop;
    Socket* listen_socket;
    InetAddress* server_address;
    Channel* accept_channel;
public:
    Acceptor(EventLoop* loop);
    void accept_connection();
};