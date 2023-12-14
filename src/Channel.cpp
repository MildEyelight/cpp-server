#include "Channel.h"
#include "Epoll.h"

int Channel::enable_reading(){
    this->events = EPOLLIN | EPOLLET;
    int rc = ep->update_channel(this);
    if(rc == -1){
        // error process;
        return rc;
    }
    this->set_is_in_epoll();
    return rc; 
}