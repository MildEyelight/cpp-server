#pragma once
#include <functional>
#include "Server.h"
#include <memory>
#include "Channel.h"
#include "Socket.h"
class Connection{
private:
	EventLoop* loop;
	Socket* sock;
	std::shared_ptr<Channel> channel;
public:
	Connection(EventLoop* _loop, Socket* _sock);
	~Connection();
	int echo(const int sockfd);
}
;
