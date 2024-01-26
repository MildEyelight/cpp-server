#pragma once
#include <functional>
#include "Server.h"
#include <memory>
#include "Channel.h"
#include "Socket.h"
#include "Buffer.h"
class Connection{

private:
	EventLoop* loop;
	Socket* sock;
	std::shared_ptr<Channel> channel;
	Buffer read_buffer;
private:
	int send_message();
	int clear_buffer();
	std::function<int(int)> disconnect_cb;
public:
	int set_disconnect_cb(std::function<int(int)> cb){
		disconnect_cb = cb;
		return 0;
	}
	Connection(EventLoop* _loop, Socket* _sock);
	int disconnect();
	~Connection();
	int echo(const int sockfd);
}
;
