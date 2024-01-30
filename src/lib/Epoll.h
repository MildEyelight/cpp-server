#pragma once
#include <sys/epoll.h>
#include <vector>
#include "Socket.h"
#include "Channel.h"

const int MAX_EVENTS = 128;

class Epoll{
private:
    int _epfd;
    struct epoll_event ev;
    struct epoll_event events[MAX_EVENTS];
    std::vector<Socket*> client_socket;
    std::vector<Channel*> listen_channel;
public:
    Epoll(){
        _epfd = epoll_create(1024);
    }
    Epoll(int epfd):_epfd(epfd){}
    Epoll(const Epoll& a) = delete;
    
    ~Epoll(){
        for(int i = 0; i < (int)client_socket.size(); i ++){
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
    int update_channel(Channel* ch){
        bzero(&ev, sizeof(ev));
        ev.data.ptr = ch;
        ev.events = ch->get_events();
        if(ch->get_is_in_epoll()){
            printf("Update fd %d status in epoll\n", ch->get_fd());
            return epoll_ctl(_epfd, EPOLL_CTL_MOD, ch->get_fd(), &ev);
        }
        else{
            printf("[SYSTEM INFO]Add fd %d to epoll\n", ch->get_fd());
            return epoll_ctl(_epfd, EPOLL_CTL_ADD, ch->get_fd(), &ev);
        }

    }
    int remove_channel(Channel* ch){
        printf("[SYSTEM INFO]Remove fd %d from epoll\n", ch->get_fd());
        return epoll_ctl(_epfd, EPOLL_CTL_DEL, ch->get_fd(), nullptr);
    }
    std::vector<Channel*> wait(){
        std::vector<Channel*> active_channels;
        int nfds = epoll_wait(_epfd, events, MAX_EVENTS, -1);
        for(int i = 0; i < nfds; i++){
            Channel* ch = (Channel*) events[i].data.ptr;
            ch->set_revents(events[i].events); 
            active_channels.push_back(ch);
        }
        return active_channels;
    }
    const struct epoll_event* get_events() const{
        return events;
    }
};