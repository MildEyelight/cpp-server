#pragma once
#include <sys/epoll.h>
#include <functional>

class Epoll;
class EventLoop;
class Channel{
private:
	EventLoop* loop;
	int fd; // TODO: Why not Socket ? 
	uint32_t events;
	uint32_t revents;
	bool is_in_epoll = false;
	std::function<void()> cb;
public:
	Channel(EventLoop* loop, int fd): loop(loop), fd(fd){}
	~Channel();
	int get_fd() const {
		return fd;
	}
	EventLoop* get_loop() const {
		return loop;
	}
	uint32_t get_events() const {
		return events;
	}
	uint32_t get_revents() const{
		return revents;
	}
	bool get_is_in_epoll() const{
		return is_in_epoll;
	}
	bool set_is_in_epoll(bool flag = true) {
		is_in_epoll = true;
		return flag;
	}
	void set_revents(const uint32_t rev) {
		revents = rev;
		return;
	}
	int disable_reading();
	int enable_reading();
	void set_callback(std::function<void()>& cb);
	void handle_event();
};
	

