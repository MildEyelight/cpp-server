#pragma once

#include "Epoll.h"
#include "Channel.h"
#include <memory>
#include <unordered_map>

class Connection;
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
    std::unordered_map<int, Connection*> connections;

public:
    Server(std::shared_ptr<EventLoop>& loop);
    ~Server();


    //Acceptor and Connection(Listen Event and TCP Event)
    void new_connection(Socket *server_socket);
    int add_connection(const int sockfd, Connection* new_connection);
    int remove_connection(const int sockfd);
    //Handler
    //After Spliting TCP into Connection object, the read event move into Connection.
    //int handle_read_event(int fd);
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