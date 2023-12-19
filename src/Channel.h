#pragma once
#include <sys/epoll.h>
#include <functional>

class Epoll;
class EventLoop;
class Channel{
private:
	Epoll *ep;
	EventLoop* loop;
	int fd;
	uint32_t events;
	uint32_t revents;
	bool is_in_epoll;
	std::function<void()> cb;
public:
	Channel(Epoll* ep, int fd):ep(ep), fd(fd){}
	Channel(EventLoop* loop, int fd): loop(loop), fd(fd){}
	int get_fd() const {
		return fd;
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
	Epoll* get_ep() const {
		return ep;
	}
	void set_revents(const uint32_t rev) {
		revents = rev;
		return;
	}
	int enable_reading();
	void set_callback(std::function<void()>& cb);
	void handle_event();
};
	

