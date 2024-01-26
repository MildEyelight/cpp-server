#include "Channel.h"
#include "Epoll.h"
#include "Server.h"
Channel::~Channel(){
    disable_reading(); 
}
int Channel::enable_reading(){
    this->events = EPOLLIN | EPOLLET;
    int rc = loop->update_channel(this);
    //int rc = ep->update_channel(this);
    if(rc == -1){
        printf("Error enable reading with rc :%d", rc);
        return rc;
    }
    this->set_is_in_epoll();
    return rc; 
}
int Channel::disable_reading(){
    int rc = 0;
    if(is_in_epoll){
        rc = loop->remove_channel(this);
        this->set_is_in_epoll(false);
    } 
    return rc;
}
void Channel::set_callback(std::function<void()>& cb){
    this->cb = cb;
}
void Channel::handle_event(){
    cb();
}