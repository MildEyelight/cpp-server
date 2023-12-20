#include "Server.h"
#include <memory>
int main(){
    std::shared_ptr<EventLoop> loop = std::make_shared<EventLoop>();
    std::shared_ptr<Server> server = std::make_shared<Server>(loop);
    loop->loop();
    return 0;
}