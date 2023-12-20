#pragma once
#include "Epoll.h"
#include "Channel.h"
#include <memory>
class EventLoop{
private: 
    std::shared_ptr<Epoll> ep;
    bool quit;
public:
    EventLoop();
    ~EventLoop();
    void loop();
    int update_channel(Channel* channel);
    std::shared_ptr<Epoll> get_epoll() const;
}
;

class Server{
private:
    //Reactor
    std::shared_ptr<EventLoop> loop;
    std::shared_ptr<class Acceptor> acceptor; 
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
    std::function<void(Socket*)> new_connection_cb;
public:
    Acceptor(EventLoop* loop);
    void set_cb(std::function<void(Socket*)>& cb){
        new_connection_cb = cb;
    }
    void accept_connection();
};