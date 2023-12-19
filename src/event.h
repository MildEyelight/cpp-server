#pragma once
#include <string>
#include "Socket.h"
const int MAX_BUFFER_SIZE = 1024;
const std::string SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 10001; 
int handle_event(int client_fd);
int client_event(const Socket& sock);
int client_event(int server_fd);