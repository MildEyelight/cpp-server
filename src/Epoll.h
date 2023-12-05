#pragma once
#include <sys/epoll.h>
#include <vector>
#include "Socket.h"

const int MAX_EVENTS = 128;

class Epoll{
private:
    int _epfd;
    struct epoll_event ev;
    struct epoll_event events[MAX_EVENTS];
    std::vector<Socket*> client_socket;
public:
    Epoll(){
        _epfd = epoll_create(1024);
    }
    Epoll(int epfd):_epfd(epfd){}
    Epoll(const Epoll& a) = delete;
    
    ~Epoll(){
        for(int i = 0; i < client_socket.size(); i ++){
            delete client_socket.at(i);
        }
    }
    
    Epoll& operator=(const Epoll& a) = delete;
    
    const int get_epfd() const{
        return _epfd;
    }
    int add_fd(const int fd, int flag = EPOLLIN){   
        ev.data.fd = fd;
        ev.events = flag;
        return epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev);
    }
    int add_socket(Socket* a, int flag = EPOLLIN){
        client_socket.push_back(a);
        return add_fd(a->get_sockfd(), flag);
    }
    int add_socket(const Socket& a, int flag = EPOLLIN){
        return add_fd(a.get_sockfd(), flag);
    }

    int wait(){
        return epoll_wait(_epfd, events, MAX_EVENTS, -1);
    }
    const struct epoll_event* get_events() const{
        return events;
    }
};