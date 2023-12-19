#include "Channel.h"
#include "Epoll.h"
#include "Server.h"
int Channel::enable_reading(){
    this->events = EPOLLIN | EPOLLET;
    int rc = loop->update_channel(this);
    //int rc = ep->update_channel(this);
    if(rc == -1){
        // error process;
        return rc;
    }
    this->set_is_in_epoll();
    return rc; 
}

void Channel::set_callback(std::function<void()>& cb){
    this->cb = cb;
}
void Channel::handle_event(){
    cb();
}